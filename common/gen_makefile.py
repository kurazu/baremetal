#!/usr/bin/env python3

import io
import os
import glob
import fnmatch
import re
import sys

SOURCE_DIR = 'source'
C_SOURCES = os.path.join(SOURCE_DIR, '*.c')
ASM_SOURCES = os.path.join(SOURCE_DIR, '*.s')
LINUX_PATTERN = '*.linux.*'
BAREMETAL_PATTERN = '*.baremetal.*'
HEADER_PATTERN = re.compile(r'#include\s+\"(\w+\.h)\"')


def get_header_dependencies(c_file_path):
    deps = []
    with io.open(c_file_path, 'r', encoding='utf-8') as c_file:
        for line in c_file:
            line = line.strip()
            match = HEADER_PATTERN.match(line)
            if not match:
                continue
            header_file_name = match.group(1)
            deps.append(os.path.join(SOURCE_DIR, header_file_name))
    return deps


class CompilationUnit(object):
    unit_type = 'CompilationUnit'

    def __init__(self, file_path, target_file_name):
        self.file_path = file_path
        self.target_file_name = target_file_name
        self.dependencies = self.get_dependencies()

    def get_dependencies(self):
        return [self.file_path]

    def __repr__(self):
        return (
            '{self.__class__.__name__}('
            '{self.file_path!r}, {self.target_file_name!r}, '
            '{self.dependencies})'
        ).format(self=self)

    def render(self, target_path, baremetal):
        return '{file_path}: {deps}\n\t{compilation_command}\n'.format(
            file_path=self.get_target_path(target_path),
            deps=' '.join(self.dependencies),
            compilation_command=self.get_compilation_command(
                target_path, baremetal
            )
        )

    def get_target_path(self, target_directory):
        return os.path.join(target_directory, self.target_file_name)


class CCompilationUnit(CompilationUnit):

    def get_dependencies(self):
        result = super().get_dependencies()
        result.extend(get_header_dependencies(self.file_path))
        return result

    def get_compilation_command(self, target_directory, baremetal):
        return '$(ARMGNU)gcc {options}-c {file_path} -o {target_path}'.format(
            options='$(COPS) ' if baremetal else '',
            file_path=self.file_path,
            target_path=self.get_target_path(target_directory)
        )


class ASMCompilationUnit(CompilationUnit):

    def get_compilation_command(self, target_directory, baremetal):
        return '$(ARMGNU)as {file_path} -o {target_path}'.format(
            file_path=self.file_path,
            target_path=self.get_target_path(target_directory)
        )


def main(app_name):
    common_files = []
    linux_files = []
    baremetal_files = []

    def process(file_path, compilation_unit_file):
        file_name = os.path.basename(file_path)
        if fnmatch.fnmatch(file_path, LINUX_PATTERN):
            target_file_name = file_name.replace('.linux.', '.')
            collection = linux_files
        elif fnmatch.fnmatch(file_path, BAREMETAL_PATTERN):
            target_file_name = file_name.replace('.baremetal.', '.')
            collection = baremetal_files
        else:
            target_file_name = file_name
            collection = common_files

        target_file_title, file_ext = os.path.splitext(target_file_name)
        target_file_name = '{}.o'.format(target_file_title)
        compilation_unit = compilation_unit_file(file_path, target_file_name)
        collection.append(compilation_unit)

    for file_path in glob.iglob(C_SOURCES):
        process(file_path, CCompilationUnit)

    for file_path in glob.iglob(ASM_SOURCES):
        process(file_path, ASMCompilationUnit)

    print(
        'ARMGNU ?= /home/kurazu/tmp/tools/arm-bcm2708/'
        'arm-bcm2708-linux-gnueabi/bin/arm-bcm2708-linux-gnueabi-'
    )
    print('COPS = -Wall -Werror -nostdlib -nostartfiles -ffreestanding')
    print()
    print(
        'all: build.linux build.linux/{app_name} '
        'build.baremetal build.baremetal/{app_name}.img\n'.format(
            app_name=app_name
        )
    )
    print('build.linux:\n\tmkdir build.linux\n')
    print('build.baremetal:\n\tmkdir build.baremetal\n')

    linux_compilation_units = common_files + linux_files
    for compilation_unit in linux_compilation_units:
        print(compilation_unit.render('build.linux', False))
    print((
        'build.linux/{app_name}: build.linux {deps}\n'
        '\t$(ARMGNU)gcc {deps} -o build.linux/{app_name}\n'
    ).format(
        app_name=app_name,
        deps=' '.join(
            compilation_unit.get_target_path('build.linux')
            for compilation_unit in linux_compilation_units
        )
    ))

    baremetal_compilation_units = common_files + baremetal_files
    for compilation_unit in baremetal_compilation_units:
        print(compilation_unit.render('build.baremetal', True))
    print((
        'build.baremetal/{app_name}.img: build.baremetal '
        'build.baremetal/{app_name}.elf\n'
        '\t$(ARMGNU)objcopy build.baremetal/{app_name}.elf '
        '-O binary build.baremetal/{app_name}.img\n'
    ).format(
        app_name=app_name
    ))
    print((
        'build.baremetal/{app_name}.elf: '
        'build.baremetal {deps} source/kernel.ld\n'
        '\t$(ARMGNU)ld --no-undefined {deps} -Map build.baremetal/kernel.map '
        '-T source/kernel.ld -o build.baremetal/{app_name}.elf\n'
        '\t$(ARMGNU)objdump -D build.baremetal/{app_name}.elf '
        '> build.baremetal/{app_name}.list\n'
    ).format(
        app_name=app_name,
        deps=' '.join(
            compilation_unit.get_target_path('build.baremetal')
            for compilation_unit in baremetal_compilation_units
        )
    ))

    print('clean:\n\trm -rf build.linux build.baremetal\n')


if __name__ == '__main__':
    main(sys.argv[1])

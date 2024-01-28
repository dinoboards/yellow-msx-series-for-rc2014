### FDISK

Utility to partition and format MSXDOS/NEXTOR device drivers.  It is ported from NEXTOR's BASIC utility.

### Extract from NEXTOR documentation:

The tool has a user interface based on menus, so anyone should be able to use it by just following the indications provided in the screen (when in doubt, look for an indication on what to do next in the lower line of the screen). There are however some points of interest to consider that are not mentioned in the tool itself:

* The tool allows creating up to 256 FAT12 and FAT16 partitions on any block device attached to a Nextor device-based driver. MSX-DOS drivers and Nextor drive-based drivers are not supported.

* With this tool it is not possible to add new partitions to an already partitioned device. All existing partitions must be removed before defining new partitions.

* Partitions from 100KB (the minimum supported partition size) up to 32MB will be FAT12, partitions from 33MB to 4GB (the maximum supported partition size) will be FAT16.

* Partitions of 16MB or less will have three sectors per FAT or less, therefore they can be used in MSX-DOS 1 mode.

* Partitions up to 32MB will have a MSX-DOS 2 boot sector, partitions of 33MB and more will have a standard boot sector.

* To get an optimum cluster size, it is recommended to define the partition sizes as powers of two (that is: 1M, 2M, 4M, 8M, 16M or 32M for FAT12 partitions; 64M, 128M, 256M, 512M, 1G, 2G or 4G for FAT16 partitions). If this is not possible, it is better to select the partition size as slightly smaller than the closest power of two than slightly higher (that is, for example 31M is better than 33M).

Remember that Nextor can handle devices with FAT16 partitions and standard boot sectors; if you use a factory-partitioned device of 2GB or less you probably don't need to partition it, unless you want to create MSX-DOS 1 compatible partitions (4GB devices are usually shipped with a FAT32 partition, so you will need to partition it with FDISK anyway).

When creating new partitions you can choose which one(s) will have the "active" flag set, thus being eligible for automatic mapping at boot time (see _[3.2. Booting Nextor](#32-booting-nextor)_); it is also possible change the flag on already existing partitions.

The partitioning tool works in MSX-DOS 1 mode too. Note however that the tool will always allow you to create partitions larger than 16M, which are not compatible with MSX-DOS 1.



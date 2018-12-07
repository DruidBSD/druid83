To create a bootable USB device:

   Before plugging in your USB device:
   1. Execute: sysctl kern.disks
   2. Take note of the available devices

   After plugging in your USB device:
   1. [Re-]Execute: sysctl kern.disks
   2. Take note of the newly-available `da#' device (e.g., `da1')

   To write the ISO file to your USB device, execute:

	sudo ./iso2usb.sh ../path/to/file.iso da#

   Taking care to replace `da#' with the proper device-name.

To create a bootable CD-ROM:

   If your system has only one CD/DVD burner AND you are running FreeBSD-8.x
   or higher, simply insert a blank CD-R/RW disc into the drive and execute:

	sudo ./iso2cd.sh ../path/to/file.iso

   NOTE: The CD/DVD burner device will be auto-probed and used, if possible.

   Otherwise, you will need to specify which CD/DVD burner to use. First,
   execute the following command to list available devices:

	cdrecord -scanbus

   The output may resemble the following:

	0,0,0     0) 'HL-DT-ST' 'DVDRAM GSA-E50L ' 'NE01' Removable CD-ROM
	0,1,0     1) 'Intel(R)' 'RMM2 VDrive 1' '0.01' Removable CD-ROM
	0,2,0     2) 'Intel(R)' 'RMM2 VDrive 2' '0.01' Removable CD-ROM
	0,3,0     3) 'Intel(R)' 'RMM2 VDrive 3' '0.01' Removable CD-ROM
	0,4,0     4) 'Intel(R)' 'RMM2 VDrive 4' '0.01' Removable CD-ROM
	0,5,0     5) *
	0,6,0     6) *
	0,7,0     7) *

   In the above output, the device-name for our USB removable CD/DVD burner is
   `0,0,0'. To burn the ISO file to a CD-R/RW disc in this device, execute:

	sudo ./iso2cd.sh ../path/to/file.iso 0,0,0

To create a bootable DVD-ROM:

   On FreeBSD-4.x:
   1. Execute: ls -ltr /dev/rcd*c
   2. Each `rcd#c' name represents a valid device to try below

   On FreeBSD-8.x:
   1. Execute: ls -ltr /dev/cd*
   2. Each `cd#' name represents a valid device to try below

   To write the ISO file to your USB device, execute:

	sudo ./iso2dvd.sh ../path/to/file.iso NAME

   Taking care to replace `NAME' with the appropriate device-name.

   For example, on FreeBSD-4.x:

	sudo ./iso2dvd.sh ../path/to/file.iso rcd0c

   or on FreeBSD-8.x:

	sudo ./iso2dvd.sh ../path/to/file.iso cd0


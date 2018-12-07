To create a bootable USB device:

   Before plugging in your USB device:
   1. Execute: ls -ltr /dev/sd*
   2. Take note of the available devices

   After plugging in your USB device:
   1. [Re-]Execute: ls -ltr /dev/sd*
   2. Take note of the newly-available `sd?' device (e.g., `sdb')

   To write the ISO file to your USB device, execute:

	sudo ./iso2usb.sh ../path/to/file.iso sd?

   Taking care to replace `sd?' with the proper device-name.

To create a bootable CD-ROM:

   If your system has only one CD/DVD burner, simply insert a blank CD-R/RW
   disc into the drive and execute:

	sudo ./iso2cd.sh ../path/to/file.iso

   NOTE: The CD/DVD burner device will be auto-probed and used, if possible.

   Otherwise, you will need to specify which CD/DVD burner to use. First,
   execute the following command to list available devices:

	ls -ltr /dev/cdwriter*

   Each device represents a valid device to try below.

   To burn the ISO file to a CD-R/RW disc in this device, execute:

	sudo ./iso2cd.sh ../path/to/file.iso cdwriter

To create a bootable DVD-ROM:

   First, execute:

	ls -ltr /dev/cdwriter*

   Each `cdwriter' or `cdwriter#' name represents a valid device to try below.

   To write the ISO file to your USB device, execute:

	sudo ./iso2dvd.sh ../path/to/file.iso NAME

   Taking care to replace `NAME' with the appropriate device-name.

   For example:

	sudo ./iso2dvd.sh ../path/to/file.iso cdwriter


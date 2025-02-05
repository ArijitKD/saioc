# saioc
A simple all-in-one compiler interface for Termux environment in Android targetting educational purposes and quick prototyping.

## Installation
Download saioc from the Releases section to your Downloads folder (/storage/emulated/0/Download). To make saioc available as a Termux command, copy it to $PATH using the following command in Termux:
```
cp /storage/emulated/0/Download/saioc $PATH/saioc
```
If you get a permission denied error while running saioc, give executable permission to saioc using the Termux command:
```
chmod +x $PATH/saioc
```
saioc should now work as expected.

## Additional Info
* saioc was only tested to work using Termux.
* saioc is NOT a compiler, but a COMPILER INTERFACE for easy use, mainly for educational purposes and quick prototyping. As such, you need to have the compilers installed for the respective programming languages for saioc to work.

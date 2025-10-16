# LoRA DAQ Hardware
Here you will find the KiCAD project files for the lora daq module. This project has been separated into three main parts:  
* `adc.kicad_sch` includes the input filter and supporting hardware related to the adc  
* `power_management.kicad_sch` includes the battery controller, ldos, and dc-dc converters  
* `lora_module.kicad_sch` includes the wio-e5, lora antennae, and eeprom  

## KiCAD Version Control Notes (Please read these!)
KiCAD version control will be managed through this git repository, there are some important things to 
keep track of while you work!  

### Git Management
To keep things nice, please keep all of your work confined to the schematic for your module. This should also 
correspond to your git branch. Here's an example:

Say I'm working on the ADC, first I'll make a git branch for my work...
```
$ git checkout -b schematic-adc
Switched to a new branch 'schematic-adc'
```
then I'll do work in the adc.kicad_sch file, and when I'm ready to submit the updated schematic I'll upload it to
my schematic branch.
```
$ git add ./adc.kicad_sch
$ git commit -m "added buffer to the input filter of the ads1115"
$ git push --set-upstream origin schematic-adc
Enumerating objects: 22, done.
Counting objects: 100% (22/22), done.
Delta compression using up to 14 threads
Compressing objects: 100% (20/20), done.
Writing objects: 100% (20/20), 8.72 KiB | 4.36 MiB/s, done.
Total 20 (delta 5), reused 0 (delta 0), pack-reused 0
remote: Resolving deltas: 100% (5/5), completed with 1 local object.
remote: 
remote: Create a pull request for 'test' on GitHub by visiting:
remote:      https://github.com/danielkm-ee/lora_daq_module/pull/new/schematic-adc
remote: 
To github.com:danielkm-ee/lora_daq_module.git
 * [new branch]      schematic-adc -> schematic-adc
Branch 'schematic-adc' set up to track remote branch 'schematic-adc' from 'origin'.
```
This way I can push/pull changes to the adc schematic separately, so I can safely work on the adc schematic without 
the need to resolve merge conflicts.

### Symbol and footprint libraries
When you are creating KiCAD symbols, they will be stored in your local *project* directory. We want these files to 
stay local to your machine until we can all collectively incorporate them into a project symbol library, so we've set 
up the .gitignore file to prevent accidental uploads of your local kicad schematic files, this includes symbols, 
footprints, and project settings related to your local UI.

> #### Files to keep local:
> * `fp-info-cache` -- this helps kicad pull up your most recent footprints faster, this thing is disposable and kicad creates and replaces it automatically
> * all `*.pretty` files -- these are footprint library files, they group your local symbols into library groups
> * all `*.kicad_mod` files -- these are your actual footprints (called 'modern footprint files')

> It is important that these aren't added to your commits because they will cause merge conflicts.

You are free to create and manage your footprint and symbol libraries locally in any way you choose--try to be 
organized because it will help us create a shared symbol library.  


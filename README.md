old-tarrasch-chess-gui
======================

*Please note* This repository has been superceded in every possible way by the
peer tarrasch-chess-gui repository which replaces it. From November 25 2016
Tarrasch V2 (this repository) has been retired in favour of Tarrasch V3 (the
peer repository).

The Tarrasch Chess GUI V2. No longer work in progress, but a working useful chess program.
New Tarrasch development is occurring in peer repository tarrasch-chess-gui

Note the existence of branches named for releases published on www.triplehappy.com.
For example: V2.03b for a June 2015 release.

Note also the existence of branch unicode_build_by_ralph_moritz. This was the
master for a long time and I only merged my changes (from eg V2.03b) back to
master on June 30th 2015.

Also note that I've now added instructions (in directory install) on how
to create distributions of type setup.exe and portable.zip once you have managed
to build Tarrasch.exe (for which, see below).

Build Notes - Bill Forster 12Mar2010 (updated for V2, 7Jan2012)
==============================================================

Rebuilding Tarrasch from the source code delivery package is not rocket 
science, but it probably will take some application and patience, even 
from experienced developers.

I have used Microsoft Visual Studio 2010 Express as my C++ compiler and 
linker. The build package includes solution and project files generated 
by that version of Visual Studio.

It is also necessary to separately download, install and configure the 
open source WxWidgets GUI library.

I have been using V2.8.6 of this library since the inception of this 
project in mid 2008. It's installed in the default C:\wxWidgets-2.8.6 
location. I haven't checked for more recent versions of WxWidgets. I 
don't expect problems if you do use a more recent version but I can't 
dismiss the possibility out of hand.

In the Tarrasch property pages, the configuration properties explicitly 
call out specific WxWidgets directories in the Include and Library 
directories parts of each of the C/C++, the linker and resources 
sections.

If you use a different version of Visual Studio or WxWidgets and you 
find that gives you problems, I suggest the following two part strategy;

1) Put aside the Tarrasch source code for a while and concentrate on 
building simple WxWidgets samples and demos. That process should be very 
well supported, and there is no chance you can build Tarrasch if you 
can't get that far at least.

2) Once you have confidence in your compiler + WxWidgets development 
environment, take a suitable demo and start introducing the Tarrasch 
source files. That basically mirrors the process that spawned Tarrasch 
in the first place. You should quickly be able to add all the existing 
source files and effectively you've grown a new Tarrasch solution. The 
only source files in the package that aren't part of the Tarrasch GUI 
are ToyEngine.cpp which is the root file in a separate console mode app 
(the toy engine) and BuildLookupTables.cpp which is the sole file of a 
separate console mode application to build GeneratedLookupTables.inc 
(which is included in the package, you don't need to build it yourself).

* VERY IMPORTANT *
There is one really painful detail that you must get on top of. I have 
changed the Configuration Properties / C/C++ /Code Generation / Runtime 
Library field from its default Multi-threaded DLL to Multi-threaded. The 
reason is that otherwise you don't get a real fully static link. You 
want a real static link or your version of Tarrasch might fail on a PC 
with different DLL versions to yours. The catch is that YOU MUST MAKE 
THE SAME CHANGE IN THE WXWIDGETS LIBRARIES CONFIGURATION AND RECOMPILE 
THOSE TOO. If you don't you get annoying and otherwise inexplicable link 
errors. See my question on the programmers site stackoverflow.com for
a little more background (search for my name).

A note about the icon
---------------------
My (admittedly primitive and rather pathetic) method of getting Pawel
Koziol's icon to replace the standard wxWidget icon was to replace
each of std.ico child.ico mdi.ico in directory;
 C:\wxWidgets-2.8.6\include\wx\msw             
with a copy of tarrasch.ico as included in this repository. I can't
remember much about this process, but no doubt I tried to do it in
more rational ways without success.

The main problem with this (apart from the patheticness and primitiveness
already alluded too) is that this tends to give every wxWidget app
you subsequently create this inappropriate icon. No doubt a much
better solution is possible.

Finally, I remember that Windows was reluctant to give up on using the
standard wxWidgets icon for Tarrasch, it seemed to be cached deep in
the OS somewhere, and would come back in vampire like fashion, at least
for a while.



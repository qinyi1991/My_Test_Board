rd /Q /S project\MDK-ARM(uV5)\Flash
rd /Q /S project\MDK-ARM(uV5)\CpuRAM
rd /Q /S project\MDK-ARM(uV5)\ExtSRAM
del /Q project\MDK-ARM(uV5)\*.bak
del /Q project\MDK-ARM(uV5)\*.dep
del /Q project\MDK-ARM(uV5)\JLink*
del /Q project\MDK-ARM(uV5)\project.uvgui.*

del /Q project\ewarm7.10\Project.dep
del /Q project\ewarm7.10\Flash
del /Q project\ewarm7.10\CpuRAM
del /Q project\ewarm7.10\settings
rd  /Q /S project\ewarm7.10\Flash
rd /Q /S project\ewarm7.10\CpuRAM
rd /Q /S project\ewarm7.10\settings


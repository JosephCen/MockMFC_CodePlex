Robocopy.exe .\ \\cn04fiva\Datas\IT\S1IG\CLQRemTools\CLQCodeGen /E /XF *.pdb *.vshost.* *.xml Publish.cmd

rd /S /Q ..\Published
md ..\Published
Robocopy.exe \\cn04fiva\Datas\IT\S1IG\CLQRemTools\CLQCodeGen ..\Published /E

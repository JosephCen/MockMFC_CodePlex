using System;
using System.Runtime.InteropServices;


namespace DBQuerier.Common
{
    static class NativeApi
    {
        // Reference
        //    http://msdn.microsoft.com/en-us/library/windows/desktop/bb762153(v=vs.85).aspx
        //    http://www.pinvoke.net/default.aspx/shell32/ShellExecute.html

        [DllImport("shell32.dll", EntryPoint = "ShellExecute")]
        public static extern IntPtr ShellExecute(
            IntPtr hwnd,
            String lpOperation,
            String lpFile,
            String lpParameters,
            String lpDirectory,
            ShowCommands nShowCmd);
    }

    enum ShowCommands : int
    {
        SW_HIDE = 0,
        SW_SHOWNORMAL = 1,
        SW_NORMAL = 1,
        SW_SHOWMINIMIZED = 2,
        SW_SHOWMAXIMIZED = 3,
        SW_MAXIMIZE = 3,
        SW_SHOWNOACTIVATE = 4,
        SW_SHOW = 5,
        SW_MINIMIZE = 6,
        SW_SHOWMINNOACTIVE = 7,
        SW_SHOWNA = 8,
        SW_RESTORE = 9,
        SW_SHOWDEFAULT = 10,
        SW_FORCEMINIMIZE = 11,
        SW_MAX = 11
    }
}

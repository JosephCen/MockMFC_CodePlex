using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace WordAssistant
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

        // Reference
        //    
        //    http://www.pinvoke.net/default.aspx/coredll/PlaySound.html

        [DllImport("winmm.dll", EntryPoint = "PlaySound", SetLastError = true)]
        public static extern Int32 PlaySound_File(
            String pszSound,
            IntPtr hmod,
            SoundFlags fdwSound);
    }

    public enum ShowCommands : int
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

    public enum SoundFlags
    {
        /// <summary>play synchronously (default)</summary>
        SND_SYNC = 0x0000,
        /// <summary>play asynchronously</summary>
        SND_ASYNC = 0x0001,
        /// <summary>silence (!default) if sound not found</summary>
        SND_NODEFAULT = 0x0002,
        /// <summary>pszSound points to a memory file</summary>
        SND_MEMORY = 0x0004,
        /// <summary>loop the sound until next sndPlaySound</summary>
        SND_LOOP = 0x0008,
        /// <summary>don't stop any currently playing sound</summary>
        SND_NOSTOP = 0x0010,
        /// <summary>Stop Playing Wave</summary>
        SND_PURGE = 0x40,
        /// <summary>The pszSound parameter is an application-specific alias in the registry. You can combine this flag with the SND_ALIAS or SND_ALIAS_ID flag to specify an application-defined sound alias.</summary>
        SND_APPLICATION = 0x80,
        /// <summary>don't wait if the driver is busy</summary>
        SND_NOWAIT = 0x00002000,
        /// <summary>name is a registry alias</summary>
        SND_ALIAS = 0x00010000,
        /// <summary>alias is a predefined id</summary>
        SND_ALIAS_ID = 0x00110000,
        /// <summary>name is file name</summary>
        SND_FILENAME = 0x00020000,
        /// <summary>name is resource name or atom</summary>
        SND_RESOURCE = 0x00040004
    }
}

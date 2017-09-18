//------------------------------------------------------------------------------
// <copyright file="MyFirstCommand.cs" company="Company">
//     Copyright (c) Company.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

using System;
using System.ComponentModel.Design;
using System.Globalization;
using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Shell.Interop;
using Microsoft.VisualStudio.Text.Editor;
using Microsoft.VisualStudio.TextManager.Interop;
using Microsoft.VisualStudio.Editor;
using Microsoft.VisualStudio.Text;
using CLQCodeGen.ProxyAdapters;
using CLQCodeGen.Generators;
using CLQCodeGen.Helpers;

namespace CLQCodeGenExtension
{
    /// <summary>
    /// Command handler
    /// </summary>
    internal sealed class MyFirstCommand
    {
        /// <summary>
        /// Command ID.
        /// </summary>
        public const int CommandId = 0x0100;

        /// <summary>
        /// Command menu group (command set GUID).
        /// </summary>
        public static readonly Guid CommandSet = new Guid("bac2e1af-05be-4239-8eee-79ee6b824538");

        /// <summary>
        /// VS Package that provides this command, not null.
        /// </summary>
        private readonly Package package;

        /// <summary>
        /// Initializes a new instance of the <see cref="MyFirstCommand"/> class.
        /// Adds our command handlers for menu (commands must exist in the command table file)
        /// </summary>
        /// <param name="package">Owner package, not null.</param>
        private MyFirstCommand(Package package)
        {
            if (package == null)
            {
                throw new ArgumentNullException("package");
            }

            this.package = package;

            OleMenuCommandService commandService = this.ServiceProvider.GetService(typeof(IMenuCommandService)) as OleMenuCommandService;
            if (commandService != null)
            {
                var menuCommandID = new CommandID(CommandSet, CommandId);
                var menuItem = new MenuCommand(this.MenuItemCallback, menuCommandID);
                commandService.AddCommand(menuItem);
            }
        }

        /// <summary>
        /// Gets the instance of the command.
        /// </summary>
        public static MyFirstCommand Instance
        {
            get;
            private set;
        }

        /// <summary>
        /// Gets the service provider from the owner package.
        /// </summary>
        private IServiceProvider ServiceProvider
        {
            get
            {
                return this.package;
            }
        }

        /// <summary>
        /// Initializes the singleton instance of the command.
        /// </summary>
        /// <param name="package">Owner package, not null.</param>
        public static void Initialize(Package package)
        {
            Instance = new MyFirstCommand(package);
        }

        private IWpfTextViewHost GetCurrentViewHost()
        {
            var textManager = ServiceProvider.GetService(typeof(SVsTextManager)) as IVsTextManager;

            IVsTextView textView = null;
            const int mustHaveFocus = 1;

            textManager.GetActiveView(mustHaveFocus, null, out textView);

            var userData = textView as IVsUserData;

            if (userData == null)
            {
                return null;
            }
            else
            {
                var guidViewHost = DefGuidList.guidIWpfTextViewHost;
                object holder;

                userData.GetData(ref guidViewHost, out holder);

                var viewHost = (IWpfTextViewHost)holder;

                return viewHost;
            }
        }

        private string GetCurrentFilename(IWpfTextViewHost viewHost)
        {
            ITextDocument doc = null;

            viewHost.TextView.TextDataModel.DocumentBuffer.Properties.TryGetProperty(typeof(ITextDocument), out doc);

            return doc.FilePath;
        }

        /// <summary>
        /// This function is the callback used to execute the command when the menu item is clicked.
        /// See the constructor to see how the menu item is associated with this function using
        /// OleMenuCommandService service and MenuCommand class.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event args.</param>
        private void MenuItemCallback(object sender, EventArgs e)
        {
            string message = string.Format(CultureInfo.CurrentCulture, "Inside {0}.MenuItemCallback()", this.GetType().FullName);
            string title = "Generate Unit Test";

            var viewHost = GetCurrentViewHost();
            var filePath = GetCurrentFilename(viewHost);

            var targetTypeName = System.IO.Path.GetFileNameWithoutExtension(filePath).Replace("Test", "");
            var generatorAdapter = new UnitTestGeneratorAdapter();
            var request = new CallUnitTestGeneratorRequest
                              {
                                  Request = new UnitTestGenerateRequest
                                                {
                                                    ProdAssemblyName = "_default",
                                                    TargetTypeName = targetTypeName,
                                                    UnitTestFileName = filePath
                                                }
                              };
            var response = generatorAdapter.CallUnitTestGeneratorInIsolateAppDomain(request);

            if (!response.IsSuccess)
            {
                var messageStrB = new System.Text.StringBuilder(response.ErrorMessage);

                if (response.Response != null)
                {
                    messageStrB.AppendLine(response.Response.ExceptionMessage);
                }

                // Show a message box to prove we were here
                VsShellUtilities.ShowMessageBox(
                    this.ServiceProvider,
                    messageStrB.ToString(),
                    title,
                    OLEMSGICON.OLEMSGICON_INFO,
                    OLEMSGBUTTON.OLEMSGBUTTON_OK,
                    OLEMSGDEFBUTTON.OLEMSGDEFBUTTON_FIRST);
            }
            else
            {
                var fileHelper = new FileHelper();
                var generatorResponse = response.Response;

                fileHelper.SaveAndArchiveOriginal(filePath, generatorResponse.GeneratedText);
            }
        }
    }
}

using System;
using System.Configuration;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using WordAssistantLib;

namespace WordAssistant
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        #region Inner Object

        private class WindowParams
        {
            public String FinalMp3File
            { get; set; }
            public String LameFilePath
            { get; set; }
            public String[] WordArray
            { get; set; }
        }

        #endregion

        private const String mC_ConfigSection = "wordAssistantSection";

        private BackgroundWorker m_bgWorker;

        public MainWindow()
        {
            InitializeComponent();

            m_bgWorker = new BackgroundWorker();
            m_bgWorker.WorkerReportsProgress = true;
            m_bgWorker.DoWork += new DoWorkEventHandler(GenerateMp3_InBGWorker);
            m_bgWorker.ProgressChanged += new ProgressChangedEventHandler(GenerateMp3_ProgressChanged);
            m_bgWorker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(GenerateMp3_Completed);
        }

        private WindowParams GetWindowParams()
        {
            WindowParams wndParams = new WindowParams();

            wndParams.FinalMp3File = txtMp3File.Text;
            wndParams.LameFilePath = txtLameExe.Text;
            wndParams.WordArray = txtWords.Text.Split(new String[] { Environment.NewLine }, StringSplitOptions.RemoveEmptyEntries);

            return wndParams;
        }

        private Boolean ValidateInput(WindowParams wndParams)
        {
            if (File.Exists(wndParams.FinalMp3File))
            {
                MessageBoxResult msgRet = MessageBox.Show("File is existing. Do you want to delete existing file?",
                                                          "Information", MessageBoxButton.YesNo, MessageBoxImage.Information);

                if (MessageBoxResult.Yes == msgRet)
                    File.Delete(wndParams.FinalMp3File);
                else
                    return false;
            }

            if (!String.IsNullOrEmpty(wndParams.LameFilePath) && !File.Exists(wndParams.LameFilePath))
            {
                MessageBox.Show("Cannot find lame.exe under given path.", "Error");

                return false;
            }

            if (wndParams.WordArray.Length == 0)
            {
                MessageBox.Show("You must provide at lease 1 word.", "Error");

                return false;
            }

            return true;
        }

        private void GenerateMp3_InBGWorker(Object sender, DoWorkEventArgs e)
        {
            BackgroundWorker bgWorker = (BackgroundWorker)sender;
            WindowParams wndParams = (WindowParams)e.Argument;
            Exception innerEx = null;

            if (!GenerateMp3(wndParams, bgWorker, out innerEx))
                throw new Exception("Generate mp3 failed", innerEx);
        }

        private void GenerateMp3_ProgressChanged(Object sender, ProgressChangedEventArgs e)
        {
            if (!progBar.IsVisible)
                progBar.Visibility = Visibility.Visible;
            progBar.Value = Convert.ToDouble(e.ProgressPercentage);
        }

        private void GenerateMp3_Completed(Object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                Exception innerEx = null;
                String errMsg = String.Empty;

                innerEx = (e.Error.InnerException != null ? e.Error.InnerException : e.Error);
                errMsg = String.Format("Generated failed! Exception: {0}\n\n{1}", innerEx.Message, innerEx.StackTrace);

                MessageBox.Show(errMsg, "Error", MessageBoxButton.OK, MessageBoxImage.Error);
            }
            else
            {
                WriteConfig();

                MessageBox.Show("Mp3 file generated!", "Information");
            }
            progBar.Visibility = Visibility.Hidden;
            gridWholeWnd.IsEnabled = true;
        }

        private Boolean GenerateMp3(WindowParams wndParams, BackgroundWorker bgWorker, out Exception exception)
        {
            Boolean status = true;
            String[] wordArr = null;
            List<String> mp3FileList = null;
            HttpClientICiba iCibaClt = null;
            AudioJoinerLameExe audioJoiner = null;
            String tempDir = Environment.CurrentDirectory;
            String wordMp3TempDir = System.IO.Path.Combine(tempDir, "WordsAudio");
            String audioJoinTempDir = System.IO.Path.Combine(tempDir, "JoinDir");

            exception = null;
            try
            {
                Int32 wordIdx = 0;

                iCibaClt = new HttpClientICiba();
                wordArr = wndParams.WordArray;
                mp3FileList = new List<String>();

                Directory.CreateDirectory(wordMp3TempDir);
                for (Int32 i = 0; i < wordArr.Length; ++i)
                {
                    String singleWord = wordArr[i];

                    if (!String.IsNullOrEmpty(singleWord))
                    {
                        String wordMp3 = System.IO.Path.Combine(wordMp3TempDir,
                                                                String.Format("{0}_{1}.mp3", wordIdx.ToString("0000"),
                                                                              singleWord));
                        wordMp3 = iCibaClt.DownloadAudio(singleWord, wordMp3);
                        if (!String.IsNullOrEmpty(wordMp3))
                        {
                            mp3FileList.Add(wordMp3);
                            ++wordIdx;
                        }

                        if (null != bgWorker)
                            bgWorker.ReportProgress(i * 90 / wordArr.Length);
                    }
                }

                audioJoiner = new AudioJoinerLameExe();
                audioJoiner.TempDir = audioJoinTempDir;
                if (!String.IsNullOrEmpty(wndParams.LameFilePath))
                    audioJoiner.LameFilePath = wndParams.LameFilePath;
                audioJoiner.JoinAudio(mp3FileList, wndParams.FinalMp3File);

                if (null != bgWorker)
                    bgWorker.ReportProgress(100);
            }
            catch (Exception ex)
            {
                exception = ex;
                status = false;
            }
            finally
            {
                if (status)
                {
                    Directory.Delete(wordMp3TempDir, true);
                    Directory.Delete(audioJoinTempDir, true);
                }
            }

            return status;
        }

        private void ReadConfig()
        {
            Configuration config = ConfigurationManager.OpenExeConfiguration(
                System.IO.Path.Combine(Environment.CurrentDirectory, Environment.GetCommandLineArgs()[0]));
            WordAssistantSection wordAssistantSect = config.GetSection(mC_ConfigSection) as WordAssistantSection;

            if (null != wordAssistantSect)
            {
                WordAssistantConfigElement wordAssistantElement = wordAssistantSect.WordAssistantElement;

                txtMp3File.Text = wordAssistantElement.Mp3FilePath;
                txtLameExe.Text = wordAssistantElement.LameExePath;
            }
        }

        private void WriteConfig()
        {
            Configuration config = ConfigurationManager.OpenExeConfiguration(
                System.IO.Path.Combine(Environment.CurrentDirectory, Environment.GetCommandLineArgs()[0]));
            WordAssistantSection wordAssistantSect = config.GetSection(mC_ConfigSection) as WordAssistantSection;
            WordAssistantConfigElement wordAssistantElement = null;

            if (null != wordAssistantSect)
            {
                wordAssistantElement = wordAssistantSect.WordAssistantElement;
            }
            else
            {
                wordAssistantSect = new WordAssistantSection();
                wordAssistantElement = new WordAssistantConfigElement();

                wordAssistantSect.WordAssistantElement = wordAssistantElement;
                config.Sections.Add(mC_ConfigSection, wordAssistantSect);
            }
            wordAssistantElement.Mp3FilePath = txtMp3File.Text;
            wordAssistantElement.LameExePath = txtLameExe.Text;

            config.Save(ConfigurationSaveMode.Modified);
            ConfigurationManager.RefreshSection(mC_ConfigSection);
        }

        private void btnGenerate_Click(object sender, RoutedEventArgs e)
        {
            WindowParams wndParams = GetWindowParams();

            if (ValidateInput(wndParams))
            {
                gridWholeWnd.IsEnabled = false;
                m_bgWorker.RunWorkerAsync(wndParams);
            }
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            ReadConfig();
        }
    }
}

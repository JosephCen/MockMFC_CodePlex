using System;
using System.Configuration;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WordAssistant
{
    public class WordAssistantConfigElement : ConfigurationElement
    {
        private const String mC_LameExePath = "lameExe";
        private const String mC_Mp3FilePath = "mp3File";

        // Create an empty WordAssistantConfigElement instance
        public WordAssistantConfigElement()
        { }

        public WordAssistantConfigElement(String mp3FilePath, String lameExePath)
        {
            this.Mp3FilePath = mp3FilePath;
            this.LameExePath = lameExePath;
        }

        [ConfigurationProperty(mC_Mp3FilePath,
            DefaultValue = "",
            IsRequired = false)]
        public String Mp3FilePath
        {
            get { return (String)this[mC_Mp3FilePath]; }
            set { this[mC_Mp3FilePath] = value; }
        }

        [ConfigurationProperty(mC_LameExePath,
            DefaultValue = "",
            IsRequired = false)]
        public String LameExePath
        {
            get { return (String)this[mC_LameExePath]; }
            set { this[mC_LameExePath] = value; }
        }
    }

    public class WordAssistantSection : ConfigurationSection
    {
        private const String mC_WordAssistantElement = "wordAssistantElement";

        [ConfigurationProperty(mC_WordAssistantElement)]
        public WordAssistantConfigElement WordAssistantElement
        {
            get { return (WordAssistantConfigElement)this[mC_WordAssistantElement]; }
            set { this[mC_WordAssistantElement] = value; }
        }
    }
}

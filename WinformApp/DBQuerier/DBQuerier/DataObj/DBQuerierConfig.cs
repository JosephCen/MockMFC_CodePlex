using System;
using System.Collections.Generic;
using System.Configuration;
using System.IO;
using System.Reflection;
using System.Text;

namespace DBQuerier.DataObj
{
    class DBQuerierConfig
    {
        private Configuration m_Config;
        private String m_ConnectionString;
        private Int32 m_MaxFetchRowCount;

        private static DBQuerierConfig mS_SingleInstance;

        static DBQuerierConfig()
        {
            Configuration config = ConfigurationManager.OpenExeConfiguration(
                System.IO.Path.Combine(Environment.CurrentDirectory, Environment.GetCommandLineArgs()[0]));

            mS_SingleInstance = new DBQuerierConfig(config);
        }

        private DBQuerierConfig(Configuration config)
        {
            m_Config = config;

            m_ConnectionString = String.Empty;
            m_MaxFetchRowCount = -1;
        }

        private String GetAppSetting(String key, String defaultVal)
        {
            AppSettingsSection appSetSect = m_Config.AppSettings;
            KeyValueConfigurationElement appSetElem = null;

            appSetElem = appSetSect.Settings[key];
            if (appSetElem == null && defaultVal == null)
                throw new SettingsPropertyNotFoundException(String.Format("Cannot find appSetting {0}!", key));

            return (appSetElem != null ? appSetElem.Value : defaultVal);
        }

        public String[] GetQueryDefFiles()
        {
            String queryDefDir = GetAppSetting("QueryDefDirectory", null);
            String queryDefPat = GetAppSetting("QueryDefFilePattern", "*.xml");

            return Directory.GetFiles(queryDefDir, queryDefPat);
        }

        public String GetConnectionString()
        {
            if (String.IsNullOrEmpty(m_ConnectionString))
            {
                ConnectionStringsSection connStrSect = m_Config.ConnectionStrings;
                String connStrName = GetAppSetting("UsingConnection", String.Empty);

                if (connStrName != String.Empty)
                    m_ConnectionString = connStrSect.ConnectionStrings[connStrName].ConnectionString;
                else
                    m_ConnectionString = connStrSect.ConnectionStrings[0].ConnectionString;
            }

            return m_ConnectionString;
        }

        public Int32 GetMaxFetchRowCount()
        {
            if (m_MaxFetchRowCount < 0)
            {
                String rowCountStr = GetAppSetting("MaxFetchRowCount", String.Empty);
                Int32 rowCount = 0;

                if (!Int32.TryParse(rowCountStr, out rowCount))
                    rowCount = Int32.MaxValue;
                m_MaxFetchRowCount = rowCount;
            }

            return m_MaxFetchRowCount;
        }

        //public void FullObjectDefaultSetting(Object obj)
        //{
        //    Type objType = obj.GetType();
        //    PropertyInfo[] objPropertyInfos = null;
        //    String typeName = objType.Name;

        //    objPropertyInfos = objType.GetProperties(BindingFlags.Public | BindingFlags.Instance | BindingFlags.SetProperty);
        //    objPropertyInfos = 
        //    foreach (PropertyInfo eachPropInfo in objPropertyInfos)
        //    {
        //        if (eachPropInfo.DeclaringType.Equals()
        //    }
        //}

        public static DBQuerierConfig Instance
        {
            get { return mS_SingleInstance; }
        }
    }
}

using System;
using System.Data;
using System.Data.Common;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

using DBQuerier.DataObj;
using Common.Database;


namespace DBQuerier.DatabaseObj
{
    class ExQuery : BaseSqlDataQuery
    {
        private QueryDefType m_QueryDef;
        private KeyValuePair<String, String>[] m_QureyLineArr;

        private static Regex mS_QueryParmPat;
        private static DbProviderFactory mS_DbFactory;

        static ExQuery()
        {
            // match text like ' and Qty = @Qty -- {Qty}'
            mS_QueryParmPat = new Regex("--\\s*\\{([A-Za-z]\\w*)\\}\\s*$", RegexOptions.Compiled | RegexOptions.Singleline);

            mS_DbFactory = System.Data.SqlClient.SqlClientFactory.Instance;
        }

        public ExQuery(QueryDefType queryDef)
        {
            m_QueryDef = queryDef;

            this.ParseQueryStr();
        }

        private void ParseQueryStr()
        {
            String origQueryStr = m_QueryDef.QueryString;
            String[] qryLines = origQueryStr.Split(new Char[] { '\n', '\r' }, StringSplitOptions.RemoveEmptyEntries);
            List<KeyValuePair<String, String>> qryLineList = new List<KeyValuePair<String, String>>(qryLines.Length);

            foreach (String eachLine in qryLines)
            {
                Match m = mS_QueryParmPat.Match(eachLine);

                if (m.Success)
                {
                    String lineWithoutComment = eachLine.Substring(0, eachLine.Length - m.Groups[0].Value.Length);

                    qryLineList.Add(new KeyValuePair<String, String>(m.Groups[1].Value, lineWithoutComment.Trim()));
                }
                else
                {
                    qryLineList.Add(new KeyValuePair<String, String>(String.Empty, eachLine.Trim()));
                }
            }
            m_QureyLineArr = qryLineList.ToArray();
        }

        private String BuildQueryStr(SearchParameterSet[] parmSetArr)
        {
            StringBuilder qryStrB = new StringBuilder();
            IDictionary<String, SearchParameterSet> parmSetDic = null;
            
            parmSetDic = parmSetArr.ToDictionary(s => s.QueryParmDef.Name, StringComparer.Ordinal);

            foreach (KeyValuePair<String, String> eachLine in m_QureyLineArr)
            {
                if (String.IsNullOrEmpty(eachLine.Key))
                {
                    qryStrB.AppendLine(eachLine.Value);
                }
                else
                {
                    SearchParameterSet parmSet = null;

                    if (parmSetDic.TryGetValue(eachLine.Key, out parmSet))
                    {
                        if (parmSet.CanGenerateSearchSql())
                            qryStrB.AppendLine(parmSet.GetSearchSql());
                        else
                            qryStrB.AppendLine(eachLine.Value);
                    }
                }
            }
            System.Diagnostics.Debug.WriteLine(qryStrB.ToString());

            return qryStrB.ToString();
        }

        private DbParameter[] NewQueryParms(DbCommand dbCmd, SearchParameterSet[] parmSetArr)
        {
            List<DbParameter> dbParmList = new List<DbParameter>();

            foreach (SearchParameterSet eachParmSet in parmSetArr)
                dbParmList.AddRange(eachParmSet.GetDbParameters(dbCmd));

            return dbParmList.ToArray();
        }

        public DataTable RunQuery(String connStr, SearchParameterSet[] parmSets)
        {
            DbConnection dbConn = null;
            DbCommand dbCmd = null;
            DbDataReader dbReader = null;
            DataTable dataTable = null;
            ExtraColumnDef[] extraColDef = null;
            QueryResultReader qryRetReader = null;

            extraColDef = (m_QueryDef.ExtraColumn != null ? m_QueryDef.ExtraColumn : new ExtraColumnDef[0]);
            qryRetReader = new QueryResultReader(extraColDef);

            try
            {
                dbConn = mS_DbFactory.CreateConnection();
                dbConn.ConnectionString = connStr;

                dbCmd = dbConn.CreateCommand();
                dbCmd.CommandType = CommandType.Text;
                dbCmd.CommandText = BuildQueryStr(parmSets);
                dbCmd.Parameters.AddRange(NewQueryParms(dbCmd, parmSets));

                dbConn.Open();
                dbReader = dbCmd.ExecuteReader();
                dataTable = qryRetReader.BuildDataTable(dbReader);
                qryRetReader.LoadDataTable(dataTable, dbReader);
            }
            catch (Exception ex)
            {

            }
            finally
            {
                if (null != dbReader)
                {
                    if (!dbReader.IsClosed)
                        dbReader.Close();
                    dbReader = null;
                }
                if (null != dbCmd)
                {
                    dbCmd.Dispose();
                    dbCmd = null;
                }
                if (null != dbConn)
                {
                    if (dbConn.State != ConnectionState.Closed)
                        dbConn.Close();
                    dbConn = null;
                }
            }

            return dataTable;
        }

        public String QueryName
        {
            get { return m_QueryDef.QureyName; }
        }

        public QueryParmDef[] Parameters
        {
            get
            {
                if (m_QueryDef.Parameter == null)
                    m_QueryDef.Parameter = new QueryParmDef[0];
 
                return m_QueryDef.Parameter;
            }
        }

        public ContextMenuDef ContextMenuDef
        {
            get { return m_QueryDef.ContextMenu; }
        }

        #region BaseSqlDataQuery Member

        protected override String GetConnectionString()
        {
            return DBQuerierConfig.Instance.GetConnectionString();
        }

        protected override void RunQuery_Impl(SqlDataQueryContext ctx)
        {
            // Base class 'BaseSqlDataQuery' is not suitable for current class 'ExQuery'.
            throw new NotImplementedException();
        }

        #endregion

        private class QueryArg
        {
            public SearchParameterSet[] SearchParmSets
            { get; set; }

            public DataTable RetDataTable
            { get; set; }
        }
    }
}

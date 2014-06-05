using System;
using System.Data;
using System.Data.Common;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using DBQuerier.DataObj;


namespace DBQuerier.DatabaseObj
{
    class BaseSearchParameter
    {
        private QueryParmDef m_ParmDef;

        public BaseSearchParameter(QueryParmDef parmDef)
        {
            m_ParmDef = parmDef;
        }

        public Boolean CanGenerateSearchSql()
        {
            return (!String.IsNullOrEmpty(m_ParmDef.TableField));
        }

        public virtual String GetSearchSql(Int32 parmIdx)
        {
            throw new InvalidOperationException("Cannot call GetSearchSql() against BaseSearchParameter instance.");
        }

        public virtual String GetParameterName(Int32 parmIdx)
        {
            String parmName = String.Empty;

            if (parmIdx > 0)
                parmName = String.Format("@{0}_I{1}", m_ParmDef.Name, parmIdx.ToString());
            else
                parmName = String.Concat("@", m_ParmDef.Name);

            return parmName;
        }

        public DbParameter GetDbParameter(DbCommand dbCmd, Int32 parmIdx)
        {
            DbParameter dbParm = dbCmd.CreateParameter();

            dbParm.ParameterName = GetParameterName(parmIdx);
            dbParm.DbType = this.DbParameterType;
            dbParm.Value = this.ParameterValue;

            return dbParm;
        }

        public QueryParmDef QueryParmDef
        {
            get { return m_ParmDef; }
        }

        public String QueryParmName
        {
            get { return m_ParmDef.Name; }
        }

        public Object ParameterValue
        { get; set; }

        private DbType GetDbParameterType()
        {
            BaseQueryParameter qryParm = null;

            qryParm = QueryParameter.GetQueryParameter(m_ParmDef);

            return qryParm.DbParameterType;
        }

        public DbType DbParameterType
        {
            get { return GetDbParameterType(); }
        }
    }
}

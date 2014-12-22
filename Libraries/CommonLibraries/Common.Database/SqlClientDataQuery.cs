using System;
using System.Collections.Generic;
using System.Linq;
using System.Data;
using System.Data.Common;
using System.Data.SqlClient;


namespace Common.Database
{
    public abstract class SqlClientDataQuery : BaseSqlDataQuery
    {
        private SqlConnection ToSqlConnection(DbConnection dbConn)
        {
            SqlConnection sqlDbConn = dbConn as SqlConnection;

            if (sqlDbConn == null)
            {
                throw new InvalidOperationException(
                            String.Format("Method must be ran against a Sql server data source. Current DB connection is {0}.", dbConn.GetType()));
            }

            return sqlDbConn;
        }

        protected void RunSqlBulkInsert(SqlDataQueryContext ctx, String destTblName, DataTable srcDataTbl, IDictionary<String, String> colMapping)
        {
            DbConnection dbConn = ctx.DbConnection;
            SqlConnection sqlDbConn = ToSqlConnection(dbConn);
            SqlBulkCopy sqlBcp = null;

            try
            {
                sqlBcp = new SqlBulkCopy(sqlDbConn);

                sqlBcp.DestinationTableName = destTblName;
                if (colMapping != null)
                {
                    foreach (String srcCol in colMapping.Keys)
                        sqlBcp.ColumnMappings.Add(srcCol, colMapping[srcCol]);
                }
                sqlBcp.WriteToServer(srcDataTbl);
            }
            finally
            {
                if (sqlBcp != null)
                {
                    sqlBcp.Close();
                    sqlBcp = null;
                }
            }
        }
    }
}

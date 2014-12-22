using System;
using System.Collections.Generic;
using System.Data;
using System.Data.Common;
using System.Linq;


namespace Common.Database
{
    public abstract class BaseSqlDataQuery
    {
        static DbProviderFactory mS_DbObjFactory;

        static BaseSqlDataQuery()
        {
            mS_DbObjFactory = System.Data.SqlClient.SqlClientFactory.Instance;
        }

        public void RunQuery(Object argObj)
        {
            DbConnection dbConn = null;

            try
            {
                dbConn = mS_DbObjFactory.CreateConnection();
                dbConn.ConnectionString = GetConnectionString();

                dbConn.Open();

                SqlDataQueryContext ctx = new SqlDataQueryContext { ArgObj = argObj, DbConnection = dbConn };

                RunQuery_Impl(ctx);
            }
            catch (DbException ex)
            {
                throw new Exception("Query failed.", ex);
            }
            finally
            {
                if (null != dbConn)
                {
                    if (dbConn.State == ConnectionState.Open)
                        dbConn.Close();
                    dbConn.Dispose();
                    dbConn = null;
                }
            }
        }

        protected abstract void RunQuery_Impl(SqlDataQueryContext ctx);

        protected abstract String GetConnectionString();

        protected void RunSqlCommand(SqlDataQueryContext ctx, String sqlCmd, params DbParameter[] dbParmArr)
        {
            DbConnection dbConn = ctx.DbConnection;
            DbCommand dbCmd = dbConn.CreateCommand();

            try
            {
                dbCmd.CommandType = CommandType.Text;
                dbCmd.CommandText = sqlCmd;
                dbCmd.Parameters.AddRange(dbParmArr);

                dbCmd.ExecuteNonQuery();
            }
            finally
            {
                dbCmd.Dispose();
                dbCmd = null;
            }
        }

        protected void RunSqlQuery(SqlDataQueryContext ctx, out DataTable dataTbl, String sqlCmd, params DbParameter[] dbParmArr)
        {
            DbConnection dbConn = ctx.DbConnection;
            DbCommand dbCmd = dbConn.CreateCommand();
            DbDataAdapter dbAdapter = mS_DbObjFactory.CreateDataAdapter();

            try
            {
                dbCmd.CommandType = CommandType.Text;
                dbCmd.CommandText = sqlCmd;
                dbCmd.Parameters.AddRange(dbParmArr);

                dataTbl = new DataTable();

                dbAdapter.SelectCommand = dbCmd;
                dbAdapter.Fill(dataTbl);
            }
            finally
            {
                dbAdapter.Dispose();
                dbAdapter = null;
                dbCmd.Dispose();
                dbCmd = null;
            }
        }

        protected void RunSqlInsert(SqlDataQueryContext ctx, DataTable dataTbl, String insertCmd, params DbParameter[] dbParmArr)
        {
            DbConnection dbConn = ctx.DbConnection;
            DbCommand dbInsertCmd = dbConn.CreateCommand();
            DbDataAdapter dbAdapter = mS_DbObjFactory.CreateDataAdapter();

            try
            {
                dbInsertCmd.CommandType = CommandType.Text;
                dbInsertCmd.CommandText = insertCmd;
                dbInsertCmd.Parameters.AddRange(dbParmArr);
                dbAdapter.InsertCommand = dbInsertCmd;

                dbAdapter.Update(dataTbl);
            }
            finally
            {
                dbAdapter.Dispose();
                dbAdapter = null;
                dbInsertCmd.Dispose();
                dbInsertCmd = null;
            }
        }

        protected void RunSqlInsert(SqlDataQueryContext ctx, String destTblName, DataTable dataTbl, params DbParameter[] dbParmArr)
        {
            if (dbParmArr.Where(p => String.IsNullOrEmpty(p.SourceColumn)).FirstOrDefault() != null)
                throw new ArgumentException("There are one or more dbParam without SourceColumn name!");

            String insertCmdStr = String.Format("insert into [{0}]({1}) values ({2});",
                                                destTblName,
                                                String.Join(",", dbParmArr.Select(p => String.Format("[{0}]", p.SourceColumn))),
                                                String.Join(",", dbParmArr.Select(p => p.ParameterName)));
            this.RunSqlInsert(ctx, dataTbl, insertCmdStr, dbParmArr);
        }

        protected DbParameter CreateDbParameter(String parmName, DbType parmType, Object parmValue, String srcColName)
        {
            DbParameter dbParm = mS_DbObjFactory.CreateParameter();

            dbParm.ParameterName = parmName;
            dbParm.DbType = parmType;
            dbParm.Value = parmValue;
            if (!String.IsNullOrEmpty(srcColName))
                dbParm.SourceColumn = srcColName;

            return dbParm;
        }

        protected DbParameter CreateDbParameter(String parmName, DbType parmType, Object parmValue)
        {
            return this.CreateDbParameter(parmName, parmType, parmValue, String.Empty);
        }
    }

    public class SqlDataQueryContext
    {
        public Object ArgObj
        { get; set; }

        public DbConnection DbConnection
        { get; set; }
    }
}

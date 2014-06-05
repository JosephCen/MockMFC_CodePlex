using System;
using System.Data;
using System.Data.Common;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Diagnostics;

using DBQuerier.DataObj;


namespace DBQuerier.DatabaseObj
{
    class QueryResultReader
    {
        private ExtraColumnDef[] m_ExtraColDefs;
        private DataTable m_DataTable;
        private IDataColumnConverter[] m_ColumnConverters;

        public QueryResultReader(ExtraColumnDef[] extraColDefs)
        {
            m_ExtraColDefs = extraColDefs;
        }

        public DataTable BuildDataTable(DbDataReader dbReader)
        {
            DataTable dataTable = new DataTable();
            IDictionary<String, Int32> colNameSet = new SortedDictionary<String, Int32>(StringComparer.Ordinal);
            List<IDataColumnConverter> colConverterList = new List<IDataColumnConverter>();

            for (Int32 i = 0; i < dbReader.FieldCount; ++i)
            {
                dataTable.Columns.Add(dbReader.GetName(i), dbReader.GetFieldType(i));
                Debug.WriteLine(String.Format("Query Column: {0}({1})", dbReader.GetName(i), dbReader.GetFieldType(i).Name));
                colNameSet[dbReader.GetName(i)] = i;
            }

            foreach (ExtraColumnDef eachExtraCol in m_ExtraColDefs)
            {
                if (colNameSet.ContainsKey(eachExtraCol.SourceColumn))
                {
                    IDataColumnConverter colConverter = CreateColumnConverter(eachExtraCol);

                    colConverter.ColumnIndex = colNameSet[eachExtraCol.SourceColumn];
                    colConverterList.Add(colConverter);
                    dataTable.Columns.Add(eachExtraCol.ColumnName, typeof(String));
                }
            }
            m_DataTable = dataTable;
            m_ColumnConverters = colConverterList.ToArray();

            return dataTable;
        }

        public void LoadDataTable(DataTable dataTable, DbDataReader dbReader)
        {
            Int32 origRowLen = 0;
            Object[] origRowData = null;
            Int32 rowLen = 0;
            Object[] rowData = null;
            Int32 maxRowCount = DBQuerierConfig.Instance.GetMaxFetchRowCount();

            origRowLen = dataTable.Columns.Count - m_ColumnConverters.Length;
            origRowData = new Object[origRowLen];
            rowLen = dataTable.Columns.Count;
            rowData = new Object[rowLen];

            while (dataTable.Rows.Count < maxRowCount && dbReader.Read())
            {
                DataRow newRow = dataTable.NewRow();

                dbReader.GetValues(origRowData);
                Array.Copy(origRowData, rowData, origRowData.Length);
                for (Int32 i = 0; i < m_ColumnConverters.Length; ++i)
                {
                    IDataColumnConverter colConverter = m_ColumnConverters[i];
                    Object colVal = rowData[colConverter.ColumnIndex];

                    rowData[origRowLen + i] = colConverter.Convert(colVal);
                }
                newRow.ItemArray = rowData;
                dataTable.Rows.Add(newRow);
            }
            dataTable.AcceptChanges();
        }

        private IDataColumnConverter CreateColumnConverter(ExtraColumnDef columnDef)
        {
            return (new XmlColumnConverter(columnDef));
        }
    }
}

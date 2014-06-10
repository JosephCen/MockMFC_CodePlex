using System;
using System.Collections.Generic;
using System.Configuration;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using DBQuerier.DatabaseObj;
using DBQuerier.DataObj;
using DBQuerier.UIElement;
using DBQuerier.UIElement.Common;

namespace DBQuerier
{
    public partial class MainFrm : Form
    {
        private String m_DbConnStr;
        private ExQuery[] m_ExQueryArr;
        private ExQuery m_CurExQuery;
        private GridContextMenuBuilder m_GridCtxMenuBuilder;

        public MainFrm()
        {
            InitializeComponent();
        }

        private void MainFrm_Load(object sender, EventArgs e)
        {
            LoadAppConfiguration();
        }

        private void LoadAppConfiguration()
        {
            m_DbConnStr = DBQuerierConfig.Instance.GetConnectionString();
            m_ExQueryArr = ReadQueryDef(DBQuerierConfig.Instance.GetQueryDefFiles());
            FullQueryDefComboBox(m_ExQueryArr);
        }

        private void FullQueryDefComboBox(ExQuery[] exQueryArr)
        {
            foreach (ExQuery eachExQry in exQueryArr)
                cboQueryDef.Items.Add(new ExQueryComboItem(eachExQry));

            if (cboQueryDef.Items.Count > 0)
                cboQueryDef.SelectedIndex = 0;
        }

        private ExQuery[] ReadQueryDef(String[] qryDefFiles)
        {
            List<ExQuery> exQryList = new List<ExQuery>();
            System.Xml.Serialization.XmlSerializer qryDefXmlSerializer =
                new System.Xml.Serialization.XmlSerializer(typeof(QueryDefType));

            foreach (String eachXmlFile in qryDefFiles)
            {
                using (FileStream fiStream = File.OpenRead(eachXmlFile))
                {
                    QueryDefType qryDef = qryDefXmlSerializer.Deserialize(fiStream) as QueryDefType;

                    if (null != qryDef)
                        exQryList.Add(new ExQuery(qryDef));
                }
            }

            return exQryList.ToArray();
        }

        private void btnReset_Click(object sender, EventArgs e)
        {
            parmPicker.ParameterDefs = m_CurExQuery.Parameters;
            parmPicker.ResetParameterValues();
        }

        private void TestMethod()
        {
            BaseQueryParameter p = new StringParameter();

            p.ParseParameter(null, parmPicker.RawText);
        }

        private void RunQuery()
        {
            SearchParameterSet[] qryParmSets = parmPicker.GetParameterValues();

            if (null != qryParmSets)
            {
                splitConMain.Panel1.Enabled = false;
                splitConMain.Panel2.Enabled = false;

                bgwRunQuery.RunWorkerAsync(qryParmSets);
            }
        }

        private void btnQuery_Click(object sender, EventArgs e)
        {
            RunQuery();
        }

        private void cboQueryDef_SelectedIndexChanged(object sender, EventArgs e)
        {
            ExQueryComboItem curQryComboItem = (ExQueryComboItem)cboQueryDef.SelectedItem;

            m_CurExQuery = curQryComboItem.ExQuery;
            m_GridCtxMenuBuilder = new GridContextMenuBuilder(m_CurExQuery.ContextMenuDef);
            btnReset.PerformClick();
        }

        private DataRow GetGridSelectedRow()
        {
            DataGridViewRow selectedRow = null;
            DataRowView rowView = null;
            DataRow row = null;

            // Get target selected row from SelectedRows or SelectedCells.
            if (dataGridVw.SelectedRows.Count > 0)
            {
                selectedRow = dataGridVw.SelectedRows[0];
            }
            else
            {
                if (dataGridVw.SelectedCells.Count > 0)
                    selectedRow = dataGridVw.SelectedCells[0].OwningRow;
            }

            if (selectedRow != null)
            {
                rowView = selectedRow.DataBoundItem as DataRowView;
                if (rowView != null)
                    row = rowView.Row;
            }

            return row;
        }

        private void cmuDataGrid_Opening(object sender, CancelEventArgs e)
        {
            DataRow selectedRow = GetGridSelectedRow();

            if (selectedRow != null)
            {
                foreach (ToolStripItem eachMenuItem in cmuDataGrid.Items)
                {
                    GridContextMenuItem menuItemObj = (GridContextMenuItem)eachMenuItem.Tag;

                    eachMenuItem.Enabled = menuItemObj.GetMenuEnabled(selectedRow);
                }
            }
            else
            {
                e.Cancel = true;
            }
        }

        private delegate void MenuClickHandler();

        private void cmuDataGrid_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            GridContextMenuItem menuItemObj = (GridContextMenuItem)e.ClickedItem.Tag;

            this.BeginInvoke(new MenuClickHandler(menuItemObj.MenuClickWithNoParams));
        }

        private void bgwRunQuery_DoWork(object sender, DoWorkEventArgs e)
        {
            SearchParameterSet[] qryParmSets = e.Argument as SearchParameterSet[];

            if (qryParmSets != null)
            {
                DataTable dataTable = m_CurExQuery.RunQuery(m_DbConnStr, qryParmSets);

                e.Result = dataTable;
            }
        }

        private void bgwRunQuery_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error == null)
            {
                DataTable dataTable = e.Result as DataTable;

                if (dataTable != null)
                {
                    dataGridVw.DataSource = dataTable;
                    m_GridCtxMenuBuilder.BuildContextMenu(cmuDataGrid, dataTable);
                }
            }

            splitConMain.Panel1.Enabled = true;
            splitConMain.Panel2.Enabled = true;
        }
    }
}

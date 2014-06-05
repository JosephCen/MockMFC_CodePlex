using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using DBQuerier.DataObj;
using DBQuerier.DatabaseObj;


namespace DBQuerier.UIElement
{
    partial class ParmPicker : UserControl
    {
        public ParmPicker()
        {
            InitializeComponent();
        }

        private SearchParameterSet ParseParmVal(QueryParmDef qryParmDef, String parmValStr)
        {
            SearchParameterSet parmSet = null;
            BaseQueryParameter qryParmParser = null;

            try
            {
                qryParmParser = QueryParameter.GetQueryParameter(qryParmDef);
                parmSet = qryParmParser.ParseParameter(qryParmDef, parmValStr);
            }
            catch (Exception ex)
            {
                String exMsg = String.Format("Fail to parse '{0}'. \n{1}", parmValStr, ex.Message);

                System.Diagnostics.Debug.WriteLine(exMsg); // TODO

                //throw new Exception(exMsg, ex);
            }

            return parmSet;
        }

        public void ResetParameterValues()
        {
            StringBuilder parmValStrB = new StringBuilder();

            foreach (QueryParmDef eachParmDef in this.ParameterDefs)
                parmValStrB.AppendLine(eachParmDef.Name + ":");

            txtParm.Text = parmValStrB.ToString();
        }

        public SearchParameterSet[] GetParameterValues()
        {
            List<SearchParameterSet> parmValList = new List<SearchParameterSet>(this.ParameterDefs.Length);
            String parmValStr = txtParm.Text;
            String[] parmValStrArr = parmValStr.Split(new Char[] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);

            for (Int32 i = 0; i < parmValStrArr.Length; ++i)
            {
                String parmVal = parmValStrArr[i];
                QueryParmDef qryParmDef = this.ParameterDefs[i];
                Int32 splitIdx = parmVal.IndexOf(':');

                if (splitIdx > 0)
                {
                    String nameStr = parmVal.Substring(0, splitIdx).Trim();
                    String valueStr = parmVal.Substring(splitIdx + 1);

                    if (String.Equals(nameStr, qryParmDef.Name, StringComparison.Ordinal)
                        && !String.IsNullOrEmpty(valueStr))
                    {
                        SearchParameterSet parmSet = ParseParmVal(qryParmDef, valueStr);

                        if (!parmSet.IsEmpty)
                            parmValList.Add(parmSet);
                    }
                }
            }

            return parmValList.ToArray();
        }

        public QueryParmDef[] ParameterDefs
        { get; set; }

        public String RawText
        {
            get { return txtParm.Text; }
        }
    }
}

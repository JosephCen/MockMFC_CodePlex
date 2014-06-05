using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using DBQuerier.DataObj;


namespace DBQuerier.DatabaseObj
{
    class EqualSearchParameter : BaseSearchParameter
    {
        public EqualSearchParameter(QueryParmDef parmDef)
            : base(parmDef)
        { }

        public override String GetSearchSql(Int32 parmIdx)
        {
            if (String.IsNullOrEmpty(this.QueryParmDef.TableField))
                throw new InvalidOperationException("Cannot generate sql search condition without TableField!");

            return String.Format("({0} = {1})", this.QueryParmDef.TableField, GetParameterName(parmIdx));
        }
    }
}

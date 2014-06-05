using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DBQuerier.DatabaseObj
{
    interface IDataColumnConverter
    {
        String Convert(Object colValue);

        Int32 ColumnIndex
        { get; set; }
    }
}

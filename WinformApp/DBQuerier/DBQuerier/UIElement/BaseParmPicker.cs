using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace DBQuerier.UIElement
{
    public partial class BaseParmPicker : UserControl
    {
        private const Int32 mC_lblLocationXY = 3;
        private const Int32 mC_lblCharWidth = 5;
        private const Int32 mC_lblMargin = 6;

        public BaseParmPicker()
        {
            InitializeComponent();
        }
    }
}

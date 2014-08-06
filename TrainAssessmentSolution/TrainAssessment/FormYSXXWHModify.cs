using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TrainAssessment
{
    public partial class FormYSXXWHModify : Form
    {
        public FormYSXXWHModify()
        {
            InitializeComponent();
        }

        private void FormYSXXWHModify_FormClosed(object sender, FormClosedEventArgs e)
        {
            GlobalData.frmYSXXWHModify = null;
        }

        private void FormYSXXWHModify_Load(object sender, EventArgs e)
        {
            foreach (ListViewItem item in GlobalData.frmPXGL.listViewYSXXWH.Items)
            {
                if (item.Checked)
                {
                    ListViewItem new_item = new ListViewItem(new string[] { item.SubItems[0].Text, item.SubItems[1].Text, item.SubItems[2].Text, item.SubItems[3].Text, item.SubItems[4].Text, item.SubItems[5].Text});
                }
            }
        }
    }
}

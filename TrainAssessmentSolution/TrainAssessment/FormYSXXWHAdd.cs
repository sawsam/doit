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
    public partial class FormYSXXWHAdd : Form
    {
        public FormYSXXWHAdd()
        {
            InitializeComponent();
        }

        private void FormYSXXWHAdd_Load(object sender, EventArgs e)
        {
            Net.CreateSendPacket(GlobalData.connectFd, GlobalData.connectSn, GlobalData.MCC_REQUEST_USER, GlobalData.E_NO_ERR);
            Net.WriteString(new StringBuilder(""));
            Net.WriteString(new StringBuilder(""));
            Net.WriteInt(0);
            Net.SendPacket();
        }

        private void FormYSXXWHAdd_FormClosed(object sender, FormClosedEventArgs e)
        {
            GlobalData.frmYSXXWHAdd = null;
        }

        private void btnQuery_Click(object sender, EventArgs e)
        {
            Net.CreateSendPacket(GlobalData.connectFd, GlobalData.connectSn, GlobalData.MCC_REQUEST_USER, GlobalData.E_NO_ERR);
            Net.WriteString(new StringBuilder(this.cbDepartment.Text));
            Net.WriteString(new StringBuilder(this.tbName.Text));
            Net.WriteInt(Convert.ToInt32(this.tbNum.Text));
            Net.SendPacket();
        }
    }
}

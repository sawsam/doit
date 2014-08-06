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
    public partial class FormLogin : Form
    {
        public delegate void RecvMessage(int fd, int sn, int code, int err);

        public void connection_open(int fd, int sn, int code, int err)
        {
        }

        public FormLogin()
        {
            InitializeComponent();
        }

        private void FormLogin_Load(object sender, EventArgs e)
        {

        }

        private void btnLogin_Click(object sender, EventArgs e)
        {
            Net.ConnectServer(new StringBuilder("127.0.0.1"), new StringBuilder("5050"), 1, 5000, false);
        }
    }
}

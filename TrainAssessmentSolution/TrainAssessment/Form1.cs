using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace TrainAssessment
{
    public partial class FormMain : Form
    {
        public FormMain()
        {
            InitializeComponent();
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            if (Net.Start())
            {
                GlobalData.netStart = true;
                GlobalData.netThread = new Thread(new ThreadStart(NetThread));
                GlobalData.netThread.Start();
            }

            GlobalData.frmMain = this;
            GlobalData.business = new TrainAssessmentBusiness();
            GlobalData.frmLogin = new FormLogin();
            if (GlobalData.frmLogin.ShowDialog() == DialogResult.OK)
            {
                GlobalData.frmLogin = null;
            }
            else
            {
                this.Close();
            }
        }

        private delegate void RecvMessage(int fd, int sn, int code, int err);
        private void ProcessMessage(int fd, int sn, int code, int err)
        {
            string text = fd.ToString() + " " + sn.ToString() + " " + code.ToString() + " " + err.ToString();
            switch (code)
            {
                case GlobalData.MCI_CONNECTION_OPEN:
                    GlobalData.business.connection_open(fd, sn);
                    break;
                case GlobalData.MCI_CONNECTION_CLOSE:
                    GlobalData.business.connection_close(fd, sn);
                    break;
                case GlobalData.MCI_CONNECT_ERROR:
                    break;
                case GlobalData.MCS_LOGIN:
                    GlobalData.business.login_return(err);
                    break;
                case GlobalData.MCS_REQUEST_NOTICE:
                    GlobalData.business.notice_return(err);
                    break;
                case GlobalData.MCS_ADD_NOTICE:
                    GlobalData.business.add_notice_return(err);
                    break;
                case GlobalData.MCS_ACTIVE_ADD_NOTICE:
                    GlobalData.business.active_add_notice();
                    break;
                case GlobalData.MCS_DEL_NOTICE:
                    GlobalData.business.del_notice_return(err);
                    break;
                case GlobalData.MCS_ACTIVE_DEL_NOTICE:
                    GlobalData.business.active_del_notice();
                    break;
                case GlobalData.MCS_REQUEST_POLICY:
                    GlobalData.business.policy_return(err);
                    break;
                case GlobalData.MCS_ADD_POLICY:
                    GlobalData.business.add_policy_return(err);
                    break;
                case GlobalData.MCS_ACTIVE_ADD_POLICY:
                    GlobalData.business.active_add_policy();
                    break;
                case GlobalData.MCS_DEL_POLICY:
                    GlobalData.business.del_policy_return(err);
                    break;
                case GlobalData.MCS_ACTIVE_DEL_POLICY:
                    GlobalData.business.active_del_policy();
                    break;
                default:
                    break;
            }
        }

        private void NetThread()
        {
            while (GlobalData.netStart)
            {
                Net.PacketReset();
                int fd = 0, sn = 0, code = 0, err = 0;
                bool result = Net.PickPacket(ref fd, ref sn, ref code, ref err);
                while (result)
                {
                    RecvMessage rm = new RecvMessage(ProcessMessage);
                    Invoke(rm, fd, sn, code, err);
                    result = Net.PickPacket(ref fd, ref sn, ref code, ref err);
                }
                Thread.Sleep(100);
            }
        }

        private void btnZYYS_Click(object sender, EventArgs e)
        {
            if (GlobalData.frmPXGL == null)
            {
                GlobalData.frmPXGL = new FormPXGL();
            }
            GlobalData.frmPXGL.Show(this);
            this.Hide();
        }

        private void FormMain_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (GlobalData.netStart)
            {
                GlobalData.netStart = false;
                GlobalData.netThread.Join();
                Net.Stop();
            }
        }
    }
}

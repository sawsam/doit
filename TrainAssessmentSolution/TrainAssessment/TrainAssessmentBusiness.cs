using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace TrainAssessment
{
    public class TrainAssessmentBusiness
    {
        private DateTime get_date_time(string tick)
        {
            string timeStamp = tick;
            DateTime dtStart = TimeZone.CurrentTimeZone.ToLocalTime(new DateTime(1970, 1, 1));
            long lTime = long.Parse(timeStamp + "0000000");
            TimeSpan toNow = new TimeSpan(lTime);
            DateTime date_time = dtStart.Add(toNow);
            return date_time;
        }

        public void connection_open(int fd, int sn)
        {
            GlobalData.connectFd = fd;
            GlobalData.connectSn = sn;
            Net.CreateSendPacket(GlobalData.connectFd, GlobalData.connectSn, GlobalData.MCC_LOGIN, 0);
            Net.WriteString(new StringBuilder(GlobalData.frmLogin.comboBoxUserName.Text));
            Net.WriteString(new StringBuilder(GlobalData.frmLogin.textBoxPasswd.Text));
            Net.SendPacket();
        }

        public void connection_close(int fd, int sn)
        {
            GlobalData.connectFd = 0;
            GlobalData.connectSn = 0;
        }

        public void login_return(int err)
        {
            if (err == GlobalData.E_NO_ERR)
            {
                GlobalData.number = Net.ReadInt();
                GlobalData.name = Net.ReadString();
                GlobalData.department = Net.ReadString();
                GlobalData.frmLogin.DialogResult = DialogResult.OK;
            }
            else
            {
                MessageBox.Show("帐号密码错误！");
            }
        }

        public void notice_return(int err)
        {
            if (err == GlobalData.E_NO_ERR)
            {
                GlobalData.noticePage = Net.ReadInt();
                GlobalData.noticeTotalPage = Net.ReadInt();
                GlobalData.frmPXGL.lPageTZGG.Text = string.Format("第{0}页/{1}页", GlobalData.noticePage, GlobalData.noticeTotalPage);
                if (GlobalData.noticeList == null)
                {
                    GlobalData.noticeList = new List<Notice>();
                }
                GlobalData.noticeList.Clear();
                GlobalData.frmPXGL.listViewTZGG.Items.Clear();
                int count = Net.ReadInt();
                for (int i = 0; i < count; ++i)
                {
                    Notice notice = new Notice();
                    notice.id = Net.ReadInt();
                    notice.title = Net.ReadString();
                    notice.publisher = Net.ReadString();
                    DateTime date_time = this.get_date_time(Net.ReadInt().ToString());
                    notice.time = date_time.ToString();
                    notice.content = Net.ReadString();
                    GlobalData.noticeList.Add(notice);
                    ListViewItem item = new ListViewItem(new string[] {notice.title, notice.publisher, notice.time });
                    GlobalData.frmPXGL.listViewTZGG.Items.Add(item);
                }
                GlobalData.frmPXGL.noticePageUpdate();
            }
        }

        public void policy_return(int err)
        {
            if (err == GlobalData.E_NO_ERR)
            {
                GlobalData.policyPage = Net.ReadInt();
                GlobalData.policyTotalPage = Net.ReadInt();
                GlobalData.frmPXGL.lPageZCFG.Text = string.Format("第{0}页/{1}页", GlobalData.policyPage, GlobalData.policyTotalPage);
                if (GlobalData.policyList == null)
                {
                    GlobalData.policyList = new List<Notice>();
                }
                GlobalData.policyList.Clear();
                GlobalData.frmPXGL.listViewZCFG.Items.Clear();
                int count = Net.ReadInt();
                for (int i = 0; i < count; ++i)
                {
                    Notice notice = new Notice();
                    notice.id = Net.ReadInt();
                    notice.title = Net.ReadString();
                    notice.publisher = Net.ReadString();
                    DateTime date_time = this.get_date_time(Net.ReadInt().ToString());
                    notice.time = date_time.ToString();
                    notice.content = Net.ReadString();
                    GlobalData.policyList.Add(notice);
                    ListViewItem item = new ListViewItem(new string[] { notice.title, notice.publisher, notice.time });
                    GlobalData.frmPXGL.listViewZCFG.Items.Add(item);
                }
                GlobalData.frmPXGL.policyPageUpdate();
            }
        }

        public void add_notice_return(int err)
        {
            if (err == GlobalData.E_NO_ERR)
            {
                if (GlobalData.frmPXGL.linkLabelFJUP1.Visible == false)
                {
                    foreach (TreeNode node in GlobalData.frmPXGL.treeViewWDXX.Nodes)
                    {
                        if (node.Text == "我的消息")
                        {
                            foreach (TreeNode node1 in node.Nodes)
                            {
                                if (node1.Text == "信息查看")
                                {
                                    GlobalData.frmPXGL.treeViewWDXX.SelectedNode = node1;
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    GlobalData.frmPXGL.WDXX_Show();
                    GlobalData.frmPXGL.tlpXXFBTZ_Clear();
                }
                else
                {
                    try
                    {
                        GlobalData.uploadId = Net.ReadInt();
                        FileStream file = new FileStream(GlobalData.frmPXGL.linkLabelFJUP1.Text, FileMode.Open, FileAccess.Read);
                        GlobalData.uploadBytes = new byte[file.Length];
                        GlobalData.uploadLength = (int)file.Length;
                        GlobalData.uploadIndex = 0;
                        int thisLength = GlobalData.uploadLength > GlobalData.UPLOAD_UNIT_LENGTH ? GlobalData.UPLOAD_UNIT_LENGTH : GlobalData.uploadLength;
                        byte[] thisUpload = new byte[thisLength];
                        Array.Copy(GlobalData.uploadBytes, GlobalData.uploadIndex, thisUpload, 0, thisLength);
                        Net.CreateSendPacket(GlobalData.connectFd, GlobalData.connectSn, GlobalData.MCC_UPLOAD_ATTACHMENT, 0);
                        Net.WriteInt(GlobalData.uploadId);
                        Net.WriteInt(0);
                        Net.WriteString(new StringBuilder(GlobalData.uploadFileName));
                        Net.WriteBytes(thisUpload, thisLength);
                        Net.SendPacket();
                    }
                    catch (Exception)
                    {
                    }
                }
            }
        }

        public void add_policy_return(int err)
        {
            if (err == GlobalData.E_NO_ERR)
            {
                foreach (TreeNode node in GlobalData.frmPXGL.treeViewWDXX.Nodes)
                {
                    if (node.Text == "我的消息")
                    {
                        foreach (TreeNode node1 in node.Nodes)
                        {
                            if (node1.Text == "信息查看")
                            {
                                GlobalData.frmPXGL.treeViewWDXX.SelectedNode = node1;
                                break;
                            }
                        }
                        break;
                    }
                }
                GlobalData.frmPXGL.WDXX_Show();
                GlobalData.frmPXGL.tlpXXFBZC_Clear();
            }
        }

        public void active_add_notice()
        {
            GlobalData.frmPXGL.WDXX_Request_Notice(GlobalData.noticePage);
        }

        public void active_add_policy()
        {
            GlobalData.frmPXGL.WDXX_Request_Policy(GlobalData.policyPage);
        }

        public void del_notice_return(int err)
        {
           
        }

        public void del_policy_return(int err)
        {
            
        }

        public void active_del_notice()
        {
            GlobalData.frmPXGL.WDXX_Request_Notice(GlobalData.noticePage);
        }

        public void active_del_policy()
        {
            GlobalData.frmPXGL.WDXX_Request_Policy(GlobalData.policyPage);
        }

        public void train_info_return()
        {
            GlobalData.frmPXGL.llPXXXNum.Text = Net.ReadInt().ToString();
            GlobalData.frmPXGL.llPXXXName.Text = Net.ReadString();
            GlobalData.frmPXGL.llPXXXKS.Text = Net.ReadString();
            GlobalData.frmPXGL.llPXXXXK.Text = Net.ReadString();
            GlobalData.frmPXGL.llPXXXJD.Text = Net.ReadString();
            GlobalData.frmPXGL.llPXXXYear.Text = Net.ReadInt().ToString();
            GlobalData.frmPXGL.listViewPXAP.Items.Clear();
            int count = Net.ReadInt();
            for (int i = 0; i < count; ++i)
            {
                ListViewItem item = new ListViewItem(new string[] { (i+1).ToString(), Net.ReadString(), Net.ReadInt().ToString(), Net.ReadInt().ToString(), Net.ReadInt().ToString() });
                GlobalData.frmPXGL.listViewPXAP.Items.Add(item);
            }
        }

        public void train_list_return()
        {
            GlobalData.frmPXGL.listViewYSXXWH.Items.Clear();
            int count = Net.ReadInt();
            for (int i = 0; i < count; ++i)
            {
                ListViewItem item = new ListViewItem(new string[] { Net.ReadString(), Net.ReadInt().ToString(), Net.ReadString(), Net.ReadString(), Net.ReadInt().ToString() });
                GlobalData.frmPXGL.listViewYSXXWH.Items.Add(item);
            }
        }

        public void modify_train_return()
        {
            Net.CreateSendPacket(GlobalData.connectFd, GlobalData.connectSn, GlobalData.MCC_REQUEST_TRAIN_LIST, GlobalData.E_NO_ERR);
            Net.WriteString(new StringBuilder(""));
            Net.WriteString(new StringBuilder(""));
            Net.WriteInt(0);
            Net.SendPacket();
        }

        public void add_train_return()
        {
            Net.CreateSendPacket(GlobalData.connectFd, GlobalData.connectSn, GlobalData.MCC_REQUEST_TRAIN_LIST, GlobalData.E_NO_ERR);
            Net.WriteString(new StringBuilder(""));
            Net.WriteString(new StringBuilder(""));
            Net.WriteInt(0);
            Net.SendPacket();
        }

        public void del_train_return()
        {
            Net.CreateSendPacket(GlobalData.connectFd, GlobalData.connectSn, GlobalData.MCC_REQUEST_TRAIN_LIST, GlobalData.E_NO_ERR);
            Net.WriteString(new StringBuilder(""));
            Net.WriteString(new StringBuilder(""));
            Net.WriteInt(0);
            Net.SendPacket();
        }

        public void user_return()
        {
            GlobalData.frmYSXXWHAdd.listViewMain.Items.Clear();
            {
                ListViewItem item = new ListViewItem(new string[] { "", "", "", "", "" });
                GlobalData.frmYSXXWHAdd.listViewMain.Items.Add(item);
            }
            int count = Net.ReadInt();
            for (int i = 0; i < count; ++i)
            {
                string department = Net.ReadString();
                int number = Net.ReadInt();
                string name = Net.ReadString();
                ListViewItem item = new ListViewItem(new string[] { department, number.ToString(), name, "", "" });
                GlobalData.frmYSXXWHAdd.listViewMain.Items.Add(item);
            }
        }
    }
}

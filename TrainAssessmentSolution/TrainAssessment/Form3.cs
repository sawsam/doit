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
    public partial class FormPXGL : Form
    {
        public FormPXGL()
        {
            InitializeComponent();
        }

        private void FormPXGL_Load(object sender, EventArgs e)
        {
            this.tlpMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.WDXX_Init();
            this.WDXX_Show();
            this.XXDJ_Init();
            this.LKJH_Init();
        }

        private void FormPXGL_FormClosed(object sender, FormClosedEventArgs e)
        {
            this.Owner.Show();
            this.Owner.Refresh();
            GlobalData.frmPXGL = null;
        }

        private void WDXX_Init()
        {
            GlobalData.noticePage = 1;
            GlobalData.policyPage = 1;
            this.treeViewWDXX.Nodes.Clear();
            TreeNode root = new TreeNode("我的消息");
            this.treeViewWDXX.Nodes.Add(root);
            TreeNode child1 = new TreeNode("信息查看");
            TreeNode child2 = new TreeNode("信息发布");
            root.Nodes.Add(child1);
            root.Nodes.Add(child2);
            TreeNode child21 = new TreeNode("通知公告");
            TreeNode child22 = new TreeNode("政策法规");
            child2.Nodes.Add(child21);
            child2.Nodes.Add(child22);
            this.treeViewWDXX.SelectedNode = child1;
            this.treeViewWDXX.ExpandAll();
        }

        public void WDXX_Show()
        {
            this.WDXX_Request_Notice(GlobalData.noticePage);
            this.WDXX_Request_Policy(GlobalData.policyPage);
            this.splitContainerMain.Panel1.Controls.Clear();
            this.splitContainerMain.Panel1.Controls.Add(this.treeViewWDXX);
            this.treeViewWDXX.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeViewWDXX.Visible = true;
            this.treeViewWDXX_Show();
        }

        public void WDXX_Request_Notice(int page)
        {
            Net.CreateSendPacket(GlobalData.connectFd, GlobalData.connectSn, GlobalData.MCC_REQUEST_NOTICE, 0);
            Net.WriteInt(page);
            Net.SendPacket();
        }

        public void WDXX_Request_Policy(int page)
        {
            Net.CreateSendPacket(GlobalData.connectFd, GlobalData.connectSn, GlobalData.MCC_REQUEST_POLICY, 0);
            Net.WriteInt(page);
            Net.SendPacket();
        }

        private void treeViewWDXX_Show()
        {
            if (this.treeViewWDXX.SelectedNode.Text == "信息查看")
            {
                this.splitContainerMain.Panel2.Controls.Clear();
                this.splitContainerMain.Panel2.Controls.Add(this.tlpXXCK);
                this.tlpXXCK.Dock = System.Windows.Forms.DockStyle.Fill;
                this.tlpXXCK.Visible = true;
            }
            else if (this.treeViewWDXX.SelectedNode.Text == "通知公告")
            {
                this.splitContainerMain.Panel2.Controls.Clear();
                this.splitContainerMain.Panel2.Controls.Add(this.tlpXXFBTZ);
                this.tlpXXFBTZ.Dock = System.Windows.Forms.DockStyle.Fill;
                this.tlpXXFBTZ.Visible = true;
            }
            else if (this.treeViewWDXX.SelectedNode.Text == "政策法规")
            {
                this.splitContainerMain.Panel2.Controls.Clear();
                this.splitContainerMain.Panel2.Controls.Add(this.tlpXXFBZC);
                this.tlpXXFBZC.Dock = System.Windows.Forms.DockStyle.Fill;
                this.tlpXXFBZC.Visible = true;
            }
        }

        public void tlpXXFBTZ_Clear()
        {
            this.tbTitle.Text = "";
            this.rtbBody.Text = "";
        }

        public void tlpXXFBZC_Clear()
        {
            this.tbTitleZC.Text = "";
            this.rtbBodyZC.Text = "";
        }

        private void XXDJ_Init()
        {
            this.treeViewXXDJ.Nodes.Clear();
            TreeNode root = new TreeNode("信息登记");
            this.treeViewXXDJ.Nodes.Add(root);
            TreeNode child1 = new TreeNode("我的信息");
            TreeNode child2 = new TreeNode("医师信息维护");
            root.Nodes.Add(child1);
            root.Nodes.Add(child2);
            this.treeViewXXDJ.SelectedNode = child1;
            this.treeViewXXDJ.ExpandAll();
        }

        private void XXDJ_Show()
        {
            this.splitContainerMain.Panel1.Controls.Clear();
            this.splitContainerMain.Panel1.Controls.Add(this.treeViewXXDJ);
            this.treeViewXXDJ.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeViewXXDJ.Visible = true;
            this.treeViewXXDJ_Show();
        }

        private void treeViewXXDJ_Show()
        {
            if (this.treeViewXXDJ.SelectedNode.Text == "我的信息")
            {
                this.llPXXXName.Text = GlobalData.name;
                this.llPXXXNum.Text = GlobalData.number.ToString();
                this.llPXXXKS.Text = GlobalData.department;
                this.llPXXXXK.Text = "";
                this.llPXXXJD.Text = "";
                this.llPXXXYear.Text = "";
                this.splitContainerMain.Panel2.Controls.Clear();
                this.splitContainerMain.Panel2.Controls.Add(this.tlpWDPXXX);
                this.tlpWDPXXX.Dock = System.Windows.Forms.DockStyle.Fill;
                this.tlpWDPXXX.Visible = true;
                Net.CreateSendPacket(GlobalData.connectFd, GlobalData.connectSn, GlobalData.MCC_REQUEST_TRAIN_INFO, GlobalData.E_NO_ERR);
            }
            else if (this.treeViewXXDJ.SelectedNode.Text == "医师信息维护")
            {
                this.splitContainerMain.Panel2.Controls.Clear();
                this.splitContainerMain.Panel2.Controls.Add(this.tlpYSXXWH);
                this.tlpYSXXWH.Dock = System.Windows.Forms.DockStyle.Fill;
                this.tlpYSXXWH.Visible = true;
                Net.CreateSendPacket(GlobalData.connectFd, GlobalData.connectSn, GlobalData.MCC_REQUEST_TRAIN_LIST, GlobalData.E_NO_ERR);
            }
        }

        private void LKJH_Init()
        {
            this.treeViewLKJH.Nodes.Clear();
            TreeNode root = new TreeNode("轮科计划");
            this.treeViewLKJH.Nodes.Add(root);
            TreeNode child1 = new TreeNode("科室信息管理");
            TreeNode child2 = new TreeNode("科室轮转对照关系");
            root.Nodes.Add(child1);
            root.Nodes.Add(child2);
            this.treeViewLKJH.SelectedNode = child1;
            this.treeViewLKJH.ExpandAll();
        }

        private void LKJH_Show()
        {
            this.splitContainerMain.Panel1.Controls.Clear();
            this.splitContainerMain.Panel1.Controls.Add(this.treeViewLKJH);
            this.treeViewLKJH.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeViewLKJH.Visible = true;
            this.treeViewLKJH_Show();
        }

        private void treeViewLKJH_Show()
        {
            if (this.treeViewLKJH.SelectedNode.Text == "科室信息管理")
            {
                this.splitContainerMain.Panel2.Controls.Clear();
                this.splitContainerMain.Panel2.Controls.Add(this.tlpKSXX);
                this.tlpKSXX.Dock = System.Windows.Forms.DockStyle.Fill;
                this.tlpKSXX.Visible = true;
            }
            else if (this.treeViewLKJH.SelectedNode.Text == "科室轮转对照关系")
            {
                this.splitContainerMain.Panel2.Controls.Clear();
                this.splitContainerMain.Panel2.Controls.Add(this.tlpKSLZ);
                this.tlpKSLZ.Dock = System.Windows.Forms.DockStyle.Fill;
                this.tlpKSLZ.Visible = true;
            }
        }

        private void btnWDXX_Click(object sender, EventArgs e)
        {
            this.WDXX_Show();
        }

        private void treeViewWDXX_AfterSelect(object sender, TreeViewEventArgs e)
        {
            this.treeViewWDXX_Show();
        }

        private void linkLabelCKXXDetailBack_Click(object sender, EventArgs e)
        {
            this.treeViewWDXX_Show();
        }

        private void btnXXDJ_Click(object sender, EventArgs e)
        {
            this.XXDJ_Show();
        }

        private void treeViewXXDJ_AfterSelect(object sender, TreeViewEventArgs e)
        {
            this.treeViewXXDJ_Show();
        }

        private void btnLKJH_Click(object sender, EventArgs e)
        {
            this.LKJH_Show();
        }

        private void treeViewLKJH_AfterSelect(object sender, TreeViewEventArgs e)
        {
            this.treeViewLKJH_Show();
        }

        private void btnSubmit_Click(object sender, EventArgs e)
        {
            if (this.tbTitle.Text == "")
            {
                MessageBox.Show("标题不能为空");
                return;
            }
            if (this.rtbBody.Text == "")
            {
                MessageBox.Show("正文不能为空");
                return;
            }

            Net.CreateSendPacket(GlobalData.connectFd, GlobalData.connectSn, GlobalData.MCC_ADD_NOTICE, 0);
            Net.WriteString(new StringBuilder(this.tbTitle.Text));
            Net.WriteString(new StringBuilder("医务科"));
            Net.WriteString(new StringBuilder(this.rtbBody.Text));
            Net.SendPacket();
        }

        private void listViewTZGG_ItemChecked(object sender, ItemCheckedEventArgs e)
        {
            if (e.Item.Checked)
            {
                this.llDelTZGG.Visible = true;
            }
            else
            {
                bool check = false;
                foreach (ListViewItem item in this.listViewTZGG.Items)
                {
                    if (item.Checked)
                    {
                        check = true;
                    }
                }
                if (check == false)
                {
                    this.llDelTZGG.Visible = false;
                }
            }
        }

        private void listViewTZGG_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            ListViewHitTestInfo info = this.listViewTZGG.HitTest(e.Location);
            if (info != null && info.Item != null)
            {
                info.Item.Checked = !info.Item.Checked;
                this.splitContainerMain.Panel2.Controls.Clear();
                this.splitContainerMain.Panel2.Controls.Add(this.tlpXXCKDetail);
                this.tlpXXCKDetail.Dock = System.Windows.Forms.DockStyle.Fill;
                this.tlpXXCKDetail.Visible = true;
                int index = 0;
                foreach (Notice notice in GlobalData.noticeList)
                {
                    if (index == info.Item.Index)
                    {
                        string title = string.Format("{0}\r\n", notice.title);
                        string time = string.Format("发布部门：{0} 发布时间：{1}\r\n", notice.publisher, notice.time);   
                        string content = notice.content;
                        string space = " \r\n";

                        this.rtbXXCKDetail.Text = title;
                        this.rtbXXCKDetail.Text += space;
                        this.rtbXXCKDetail.Text += time;
                        this.rtbXXCKDetail.Text += space;
                        this.rtbXXCKDetail.Text += content;

                        this.rtbXXCKDetail.SelectionStart = 0;
                        this.rtbXXCKDetail.SelectionLength = title.Length;
                        this.rtbXXCKDetail.SelectionFont = new Font("宋体", 15f, FontStyle.Bold, GraphicsUnit.Point, (Byte)0);
                        this.rtbXXCKDetail.SelectionAlignment = HorizontalAlignment.Center;

                        this.rtbXXCKDetail.SelectionStart = title.Length - 1;
                        this.rtbXXCKDetail.SelectionLength = space.Length + time.Length;
                        this.rtbXXCKDetail.SelectionFont = new Font("宋体", 10.5f, FontStyle.Regular, GraphicsUnit.Point, (Byte)0);
                        this.rtbXXCKDetail.SelectionAlignment = HorizontalAlignment.Center;

                        this.rtbXXCKDetail.SelectionStart = title.Length + space.Length + time.Length - 1;
                        this.rtbXXCKDetail.SelectionLength = space.Length + content.Length;
                        this.rtbXXCKDetail.SelectionFont = new Font("宋体", 15f, FontStyle.Regular, GraphicsUnit.Point, (Byte)0);
                        this.rtbXXCKDetail.SelectionAlignment = HorizontalAlignment.Left;
                    }
                    index++;
                }
            }
        }

        public void noticePageUpdate()
        {
            if (GlobalData.noticePage > 1)
            {
                this.llPrePageTZGG.Enabled = true;
            }
            else
            {
                this.llPrePageTZGG.Enabled = false;
            }

            if (GlobalData.noticePage < GlobalData.noticeTotalPage)
            {
                this.llNextPageTZGG.Enabled = true;
            }
            else
            {
                this.llNextPageTZGG.Enabled = false;
            }

            this.cbGotoTZGG.AutoCompleteCustomSource.Clear();
            this.cbGotoTZGG.Items.Clear();
            for (int i = 1; i <= GlobalData.noticeTotalPage; ++i)
            {
                this.cbGotoTZGG.AutoCompleteCustomSource.Add(i.ToString());
                this.cbGotoTZGG.Items.Add(i.ToString());
            }
        }

        private void llNextPageTZGG_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            this.WDXX_Request_Notice(GlobalData.noticePage + 1);
        }

        private void llPrePageTZGG_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            this.WDXX_Request_Notice(GlobalData.noticePage - 1);
        }

        private void cbGotoTZGG_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.WDXX_Request_Notice(Convert.ToInt32(this.cbGotoTZGG.SelectedItem.ToString()));
        }

        private void llDelTZGG_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Net.CreateSendPacket(GlobalData.connectFd, GlobalData.connectSn, GlobalData.MCC_DEL_NOTICE, 0);
            int pos = Net.GetWritePos();
            Net.WriteInt(0);
            int count = 0;
            foreach (ListViewItem item in this.listViewTZGG.Items)
            {
                if (item.Checked)
                {
                    Notice notice = GlobalData.noticeList[item.Index];
                    Net.WriteInt(notice.id);
                    count++;
                }
            }
            Net.SetWritePos(pos);
            Net.WriteInt(count);
            Net.SendPacket();
        }

        private void llDelZCFG_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Net.CreateSendPacket(GlobalData.connectFd, GlobalData.connectSn, GlobalData.MCC_DEL_POLICY, 0);
            int pos = Net.GetWritePos();
            Net.WriteInt(0);
            int count = 0;
            foreach (ListViewItem item in this.listViewZCFG.Items)
            {
                if (item.Checked)
                {
                    Notice notice = GlobalData.policyList[item.Index];
                    Net.WriteInt(notice.id);
                    count++;
                }
            }
            Net.SetWritePos(pos);
            Net.WriteInt(count);
            Net.SendPacket();
        }

        private void btnSubmitZC_Click(object sender, EventArgs e)
        {
            if (this.tbTitleZC.Text == "")
            {
                MessageBox.Show("标题不能为空");
                return;
            }
            if (this.rtbBodyZC.Text == "")
            {
                MessageBox.Show("正文不能为空");
                return;
            }

            Net.CreateSendPacket(GlobalData.connectFd, GlobalData.connectSn, GlobalData.MCC_ADD_POLICY, 0);
            Net.WriteString(new StringBuilder(this.tbTitleZC.Text));
            Net.WriteString(new StringBuilder("医务科"));
            Net.WriteString(new StringBuilder(this.rtbBodyZC.Text));
            Net.SendPacket();
        }

        private void listViewZCFG_ItemChecked(object sender, ItemCheckedEventArgs e)
        {
            if (e.Item.Checked)
            {
                this.llDelZCFG.Visible = true;
            }
            else
            {
                bool check = false;
                foreach (ListViewItem item in this.listViewZCFG.Items)
                {
                    if (item.Checked)
                    {
                        check = true;
                    }
                }
                if (check == false)
                {
                    this.llDelZCFG.Visible = false;
                }
            }
        }

        private void listViewZCFG_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            ListViewHitTestInfo info = this.listViewZCFG.HitTest(e.Location);
            if (info != null && info.Item != null)
            {
                info.Item.Checked = !info.Item.Checked;
                this.splitContainerMain.Panel2.Controls.Clear();
                this.splitContainerMain.Panel2.Controls.Add(this.tlpXXCKDetail);
                this.tlpXXCKDetail.Dock = System.Windows.Forms.DockStyle.Fill;
                this.tlpXXCKDetail.Visible = true;
                Notice notice = GlobalData.policyList[info.Item.Index];
                this.rtbXXCKDetail.Text = string.Format("{0}\r\n", notice.title);
                this.rtbXXCKDetail.Text += string.Format("发布部门：{0} 发布时间：{1}\r\n", notice.publisher, notice.time);
                this.rtbXXCKDetail.Text += notice.content;
            }
        }

        public void policyPageUpdate()
        {
            if (GlobalData.policyPage > 1)
            {
                this.llPrePageZCFG.Enabled = true;
            }
            else
            {
                this.llPrePageZCFG.Enabled = false;
            }

            if (GlobalData.policyPage < GlobalData.policyTotalPage)
            {
                this.llNextPageZCFG.Enabled = true;
            }
            else
            {
                this.llNextPageZCFG.Enabled = false;
            }

            this.cbGotoZCFG.AutoCompleteCustomSource.Clear();
            this.cbGotoZCFG.Items.Clear();
            for (int i = 1; i <= GlobalData.policyTotalPage; ++i)
            {
                this.cbGotoZCFG.AutoCompleteCustomSource.Add(i.ToString());
                this.cbGotoZCFG.Items.Add(i.ToString());
            }
        }

        private void llPrePageZCFG_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            this.WDXX_Request_Policy(GlobalData.policyPage - 1);
        }

        private void llNextPageZCFG_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            this.WDXX_Request_Policy(GlobalData.policyPage + 1);
        }

        private void cbGotoZCFG_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.WDXX_Request_Policy(Convert.ToInt32(this.cbGotoZCFG.SelectedItem.ToString()));
        }

        private void btnBrowse_Click(object sender, EventArgs e)
        {
            if (this.ofdUpload.ShowDialog() == DialogResult.OK)
            {
                this.tbPath.Text = this.ofdUpload.FileName;
            }
        }

        private void btnUpLoad_Click(object sender, EventArgs e)
        {
            System.IO.FileInfo file;
            try
            {
                file = new System.IO.FileInfo(this.ofdUpload.FileName);
            }
            catch (Exception)
            {
                MessageBox.Show("请选择文件");
                return;
            }

            if (this.linkLabelFJUP1.Visible == false)
            {
                this.linkLabelFJUP1.Text = file.Name;
                this.linkLabelFJUP1.Visible = true;
                this.tbPath.Clear();
                this.ofdUpload.FileName = "";
            }
            else if (this.linkLabelFJUP2.Visible == false)
            {
                this.linkLabelFJUP2.Text = file.Name;
                this.linkLabelFJUP2.Visible = true;
                this.tbPath.Clear();
                this.ofdUpload.FileName = "";
            }
            else if (this.linkLabelFJUP3.Visible == false)
            {
                this.linkLabelFJUP3.Text = file.Name;
                this.linkLabelFJUP3.Visible = true;
                this.tbPath.Clear();
                this.ofdUpload.FileName = "";
            }
        }

        private void btnYSXXWHModify_Click(object sender, EventArgs e)
        {
            GlobalData.frmYSXXWHModify = new FormYSXXWHModify();
            GlobalData.frmYSXXWHModify.ShowDialog();
        }

        private void btnYSXXWHAdd_Click(object sender, EventArgs e)
        {
            GlobalData.frmYSXXWHAdd = new FormYSXXWHAdd();
            GlobalData.frmYSXXWHAdd.ShowDialog();
        }
    }
}

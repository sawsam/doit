using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.IO;

namespace TrainAssessment
{
    public class GlobalData
    {
        public static FormMain frmMain;
        public static FormLogin frmLogin;
        public static FormPXGL frmPXGL;
        public static FormYSXXWHModify frmYSXXWHModify;
        public static FormYSXXWHAdd frmYSXXWHAdd;
        public static TrainAssessmentBusiness business;
        public static int number;
        public static string name;
        public static string department;
        public static int noticePage;
        public static int noticeTotalPage;
        public static int policyPage;
        public static int policyTotalPage;
        public static List<Notice> noticeList;
        public static List<Notice> policyList;
        public static string uploadFileName;
        public static int uploadId;
        public static byte[] uploadBytes;
        public static int uploadLength;
        public static int uploadIndex;

        public static Thread netThread;
        public static bool netStart;
        public static int connectFd;
        public static int connectSn;

        public const int UPLOAD_UNIT_LENGTH = 40960;

        //code
        public const int MCI_CONNECTION_OPEN = 10002;
	    public const int MCI_CONNECTION_CLOSE = 10003;
        public const int MCI_CONNECT_ERROR = 10004;
        public const int MCC_LOGIN = 20001; 
        public const int MCC_REQUEST_NOTICE = 20002;
	    public const int MCC_REQUEST_POLICY = 20003;
	    public const int MCC_ADD_NOTICE = 20004;
	    public const int MCC_ADD_POLICY = 20005;
	    public const int MCC_DEL_NOTICE = 20006;
	    public const int MCC_DEL_POLICY = 20007;
        public const int MCC_UPLOAD_ATTACHMENT = 20012;
        public const int MCC_REQUEST_TRAIN_INFO = 20013;
	    public const int MCC_REQUEST_TRAIN_LIST = 20014;
	    public const int MCC_MODIFY_TRAIN = 20015;
	    public const int MCC_ADD_TRAIN = 20016;
        public const int MCC_DEL_TRAIN = 20017;
        public const int MCC_REQUEST_USER = 20018;



        public const int MCS_LOGIN = 40001;
        public const int MCS_REQUEST_NOTICE = 40002;
        public const int MCS_REQUEST_POLICY = 40003;
        public const int MCS_ADD_NOTICE = 40004;
        public const int MCS_ADD_POLICY = 40005;
        public const int MCS_DEL_NOTICE = 40006;
        public const int MCS_DEL_POLICY = 40007;
        public const int MCS_ACTIVE_ADD_NOTICE = 40008;
        public const int MCS_ACTIVE_ADD_POLICY = 40009;
        public const int MCS_ACTIVE_DEL_NOTICE = 40010;
        public const int MCS_ACTIVE_DEL_POLICY = 40011;
        public const int MCS_UPLOAD_ATTACHMENT = 40012;
        public const int MCS_REQUEST_TRAIN_INFO = 40013;
	    public const int MCS_REQUEST_TRAIN_LIST = 40014;
	    public const int MCS_MODIFY_TRAIN = 40015;
	    public const int MCS_ADD_TRAIN = 40016;
        public const int MCS_DEL_TRAIN = 40017;
        public const int MCS_REQUEST_USER = 40018;

        //error
        public const int E_NO_ERR = 0;
        public const int E_PASSWORD_ERR = 1;
    }
}

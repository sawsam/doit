#ifndef ERROR_CODE_H_
#define ERROR_CODE_H_

namespace common
{
	enum ErrorCode
	{
		E_NO_ERR = 0,

		E_PASSWORD_ERR = 1,//�ʺ��������

		E_CHECKED_EXPIRE = 2,//��֤�ѹ���

		E_SERVER_EXCEPTION = 3,//�����쳣

		E_ROLE_ERROR = 4,//��ɫ������

		E_ROLE_REPEAT = 5,//��������ظ�

		E_ROOM_BUSY   = 6, //��������æ���������ʧ��

		E_BIG_HORN_NOT_ENOUGH = 7,//�����Ȳ���

		E_NOT_WANT_JOIN_ROOM = 8, //�Է�������뷿��

		E_NOT_IN_TEAM = 9,//���ڶ��飬���ܶ�������

		E_NOT_IN_ROOM = 10,//���ڷ��䣬���ܷ�������

		E_NO_RIGHT_ROOM = 11 ,//û�к��ʵķ���

		E_ROLE_NOT_EXIST = 15,//��ɫ������

		E_FRIEND_LIST_FULL = 16,//�����б�����

		E_OPPOSITE_FRIEND_LIST_FULL = 17,//�Է������б�����

		E_FRIEND_ALREADY = 18,//�Ѿ��Ǻ���

		E_BLACK_LIST_FULL = 19,//�������б�����

		E_BLACK_ALREADY = 20,//���ں�������

		E_IN_BLACK = 21,//���Լ��ĺ������У����ܼӺ���

		E_IN_OPPOSITE_BLACK = 22,//�ڶԷ��ĺ������У����ܼӺ���

		E_IN_FRIEND = 23,//�ں����б��У����ܼӺ�����

		E_IN_BLACK_NO_TALK = 24,//���Լ��ĺ������У�����˽��

		E_IN_OPPOSITE_BLACK_NO_TALK = 25,//�ڶԷ��ĺ������У�����˽��

		E_NOT_ONLINE = 26,//�Է�������

		E_ROOM_ERROR_PASSWORD = 28, //�����������

		E_ROOM_HALF_JOIN = 29, //��;���ɼ���

		E_ROOM_FULL = 30, //�����Ѿ�����

		E_ROOM_NO_EXIST = 31, //���䲻����

		E_MAP_HAS_MORE_PLAYERS = 32, //�������Ѵ�����

		E_ROOM_ERROR_CAREER = 33, //��ְҵ�����Խ��뷿��

		E_ROOM_END_SPAWN = 34, //�Ѿ��������һ������㣬��������;����

		E_ROOM_IS_CLOSE  = 35,  //�����Ѿ��ڽ�����߹ر�״̬��

		E_BAG_NOT_ENOUGH = 36, //�����ռ䲻��

		E_ITEM_SELL_OUT = 37,//����Ʒ�ѱ��������ˡ�

		E_ITEM_BUY_ENOUGH = 38,//���ѹ������޹������ĸ���Ʒ��
	};
}

#endif /* ERROR_CODE_H_ */;
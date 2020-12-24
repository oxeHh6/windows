#include "stdafx.h"
#include <iostream>
#include "CNetCardInfo.h"

#pragma comment(lib,"Iphlpapi.lib") //��Ҫ���Iphlpapi.lib��

CNetCardInfo::CNetCardInfo()
{

}

std::vector<std::shared_ptr<CNetCardInfo>> CNetCardInfo::GetNetworkInfo() {
	std::vector<std::shared_ptr<CNetCardInfo>> info_list;

	{
		/*****************��ȡ��������mac��ַ��ip��ַ���������롢Ĭ������**********************/
		//PIP_ADAPTER_INFO�ṹ��ָ��洢����������Ϣ
		PIP_ADAPTER_INFO adapter_info = new IP_ADAPTER_INFO();
		PIP_ADAPTER_INFO adapter = nullptr;
		unsigned long stSize = sizeof(IP_ADAPTER_INFO);
		memset(adapter_info, 0, stSize);
		//����GetAdaptersInfo����,���pIpAdapterInfoָ�����;����stSize��������һ��������Ҳ��һ�������
		auto ret = GetAdaptersInfo(adapter_info, &stSize);
		if (ERROR_BUFFER_OVERFLOW == ret) {
			delete adapter_info;
			adapter_info = (PIP_ADAPTER_INFO)new BYTE[stSize];
			ret = GetAdaptersInfo(adapter_info, &stSize);
		}
		if (ERROR_SUCCESS == ret) { // �����ж�����,���ͨ��ѭ��ȥ�ж�
			adapter = adapter_info; // ��������ͷ��������Ҫ�ͷſռ�
			while (adapter) {
				CNetCardInfo info;
				if (info.ParseInfo(adapter)) {
					info_list.push_back(std::make_shared<CNetCardInfo>(info)); // ת���ɹ������
				}
				adapter = adapter->Next;
			}
		}
		//�ͷ��ڴ�ռ�
		if (adapter_info) {
			delete adapter_info;
		}
	}

#if 1
  {
	  /******************��ȡ��������dns��ַ************************/

	  PIP_ADAPTER_ADDRESSES addresses = nullptr, cur_addresses = nullptr;
	  ULONG outBufLen = 15000; // Allocate a 15 KB buffer to start with.
	  ULONG Iterations = 0, ret = 0;
	  do {
		  addresses = (IP_ADAPTER_ADDRESSES *)new BYTE[outBufLen];
		  if (addresses != nullptr) {
			  // 0x07��Skip_UNICAST��Skip_ANYCAST��Skip_MULTICAST
			  ret = GetAdaptersAddresses(AF_UNSPEC, 0x07, nullptr, addresses, &outBufLen);
			  if (ret == NO_ERROR) break;
			  if (ret == ERROR_BUFFER_OVERFLOW) {
				  delete addresses;
				  addresses = nullptr;
			  }
		  }
		  Iterations++;

	  } while ((ret == ERROR_BUFFER_OVERFLOW) && (Iterations < 3));

	  if (ret == NO_ERROR) {
		  // If successful, output some information from the data we received
		  cur_addresses = addresses;
		  while (cur_addresses) {
			  auto it = std::find_if(info_list.begin(), info_list.end(), [&](auto & info)
			  //CNetCardInfo it = std::find_if(info_list.begin(), info_list.end(), [&](CNetCardInfo  & info)
			  { // ������������GUID�ҵ���Ӧ�Ķ��������Ϣ
				  return info->info_.adapter_name == cur_addresses->AdapterName;
			  });
			  
			  if (it != info_list.end()) {
				  (*it)->ParseInfo(cur_addresses);
			  }
			  else { // û�ҵ���ֻ����Loopback�͵ģ���Ϊ���ص�ַ����Ҫ
#if _DEBUG
				  std::cout << "Not found��" << cur_addresses->AdapterName << std::endl;
#endif
			  }
			  
			  cur_addresses = cur_addresses->Next;
			  }
	  }

	  if (addresses) delete addresses;
  }
#endif


	return std::move(info_list);
}

const CNetCardInfo::Info & CNetCardInfo::GetNetCardInfo()
{
	return info_;
}

void CNetCardInfo::show() {
	std::cout << "��������" << info_.adapter_name << std::endl;
	std::cout << "��������" << info_.friendly_name << std::endl;
	std::cout << "Mac��ַ��" << info_.physical_addr << std::endl;
	std::cout << "Ip��ַ��" << info_.ip << std::endl;
	std::cout << "�������룺" << info_.ip_mask << std::endl;
	std::cout << "Ĭ�����أ�" << info_.gateway << std::endl;
	std::cout << "Ĭ��Dns��" << info_.default_dns << std::endl;
	std::cout << "��ѡDns��" << info_.backup_dns << std::endl;
	std::cout << std::endl;
}

bool CNetCardInfo::SetDns(const std::string & default_dns, const std::string & backup_dns) {
	if (config_ && config_->set_dns(default_dns, backup_dns)) {
		info_.default_dns = default_dns; // ������Ϣ
		info_.backup_dns = backup_dns;
		return true;
	}
	return false;
}

bool CNetCardInfo::SetAutoDns() { // ����autodns֮����Ҫ������Ϣ��Ŀǰδ���������Զ�����
	return config_ && config_->set_auto_dns();
}

bool CNetCardInfo::SetIpConfig(const std::string & ip, const std::string & ip_mask)
{
	//���þ�̬ip֮ǰҪ������enable_dhcp, ���԰���ǰ���õ�ip�����ض����, ����ע�����ж��ip��Ϣ
	if (!config_) return false;
	config_->enable_dhcp();
	if (config_->set_ip_config(ip, ip_mask)) {
		info_.ip = ip; // ���óɹ��͸�����Ϣ
		info_.ip_mask = ip_mask;
		return true;
	}
	return false;
}

bool CNetCardInfo::SetGateway(const std::string &gateway) {
	if (!config_) return false; // ����Ĭ��������Ҫ�ھ�̬ip��ģʽ�²��ܳɹ�,��̬ģʽ����SetIpConfig��
	bool flag = true;
	if (gateway != "0.0.0.0")
		flag = config_ && config_->set_gateway(gateway); // ���Ҫ���õ�gateway��Ϊ"0.0.0.0"���Ǿ�ֱ������
	else if (info_.gateway != gateway)
		flag = config_->set_gateway(info_.ip); // ���õ�ǰ����ʹ�õ�ipΪ���أ������������
	if (flag) info_.gateway = gateway; // ��������ջ����������أ�ֻҪ�ɹ��ˣ�����Ҫ���µ�ǰ����Ϣ 
	return flag;
}

bool CNetCardInfo::ParseInfo(PIP_ADAPTER_INFO adapter_info) {
	if (!adapter_info) return false;
	if (adapter_info->Type == MIB_IF_TYPE_LOOPBACK) return false; // ���Ի��ص�ַ

	info_.adapter_name = adapter_info->AdapterName;

	char mac[128] = { 0 };
#if 1
	for (DWORD i = 0; i < adapter_info->AddressLength; i++)
		sprintf(mac + strlen(mac)
		, (i < adapter_info->AddressLength - 1 ? "%02X-" : "%02X")
		, adapter_info->Address[i]);
	info_.physical_addr = mac;
	info_.ip = adapter_info->IpAddressList.IpAddress.String; // ��Ȼһ�����������ж��ip���������룬��������ֻ��Ҫ��һ������
	info_.ip_mask = adapter_info->IpAddressList.IpMask.String;
	info_.gateway = adapter_info->GatewayList.IpAddress.String;
	//config_.reset(new NetConfig(info_.adapter_name)); // ����������GUID����һ��config�����ṩ�������Ե��޸�
#endif
	return true;
}

// ��Ҫ��ȡdns�������������ƣ�����������ص���һ��������ȡ
bool CNetCardInfo::ParseInfo(PIP_ADAPTER_ADDRESSES adapter_addresses) {
	auto dns_server = adapter_addresses->FirstDnsServerAddress;
	if (dns_server) {
		for (int count = 0; dns_server != nullptr; count++) {
			unsigned char *addr = (unsigned char *)(dns_server->Address.lpSockaddr->sa_data);
			std::string dns = "";
			for (int i = 2; i < 6; ++i) { // ��addr[2]-addr[5]תΪ�ַ�����ʾ��ÿһ���ֽھ���dns��һ��
				dns += std::to_string(addr[i]);
				if (i != 5) dns += ".";
			}
			if (!count) {
				info_.default_dns = dns;
			}
			else {
				info_.backup_dns = dns;
				break;
			}
			dns_server = dns_server->Next;
		}
	}

	info_.friendly_name = CW2A(adapter_addresses->FriendlyName); // ��ȡ����
	return true;
}
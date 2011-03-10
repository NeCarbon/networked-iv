function onResourceStart()
{
	log("Hello World\n");
}
addEventHandler("resourceStart", getRootEntity(), onResourceStart);
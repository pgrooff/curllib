
// 
// download file using curlib.
// win32 is ok
// x64 link errors.

#include "c4d.h"
#include <curl.h>

#define ID_PIM 1012956121

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

Bool downloadFile(void)
{
    CURL *curl;
    FILE *fp;
    CURLcode res;

    char *url = "http://www.grooff.eu/pictures/download.txt"; 
    char outfilename[FILENAME_MAX] = "d:/pim.txt";
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(outfilename,"w");									
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);		// write to file
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);

		/* Check for errors */ 
		if(res != CURLE_OK)
		  fprintf(stderr, "curl_easy_perform() failed: %s\n",
				  curl_easy_strerror(res));

        curl_easy_cleanup(curl);
        fclose(fp);
    }
    return true;
} // end downloadFile()

class MainDialog : public GeDialog
{
private:


public:
	MainDialog(void);
	virtual ~MainDialog(void);

	virtual Bool CreateLayout(void);
	virtual Bool InitValues(void);
	virtual Bool Command(Int32 id, const BaseContainer& msg);
	virtual Int32 Message(const BaseContainer& msg, BaseContainer& result);
};

MainDialog::MainDialog(void)
{
}

MainDialog::~MainDialog(void)
{
}

Bool MainDialog::CreateLayout(void)
{
	AddButton(1000, BFH_SCALEFIT, 120, 20, "Do Command");

	return true;
}

Bool MainDialog::InitValues(void)
{
	return true;
}

Bool MainDialog::Command(Int32 id, const BaseContainer& msg)
{
	switch (id) 
	{
		case 1000:		//do command	
		{
		  downloadFile();
		  break;
		}
	}
	return true;
}

Int32 MainDialog::Message(const BaseContainer& msg, BaseContainer& result)
{
	//switch (msg.GetId())
	//{
	//}
	return GeDialog::Message(msg, result);
}

class MyPlugin : public CommandData
{
private:
	MainDialog dlg;

public:
	virtual Bool Execute(BaseDocument* doc);
	virtual Bool RestoreLayout(void* secret);
};

Bool MyPlugin::Execute(BaseDocument* doc)
{
	return dlg.Open(DLG_TYPE_ASYNC, ID_PIM, -1, -1); 
}

Bool MyPlugin::RestoreLayout(void* secret)
{
	return dlg.RestoreLayout(ID_PIM, 0, secret);
}

Bool RegisterMyPlugin(void)
{
	return RegisterCommandPlugin(ID_PIM, "Simpel", 0, AutoBitmap("icon.tif"), String("Simpel"), NewObjClear(MyPlugin));
}


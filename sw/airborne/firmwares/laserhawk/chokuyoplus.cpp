#include "chokuyoplus.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHokuyoPlus::CHokuyoPlus(char * deviceNameInit)
{
    sprintf(deviceName,"%s",deviceNameInit);
    //deviceName = strdup(deviceNameInit);

    data.angle = (FLOATHOKUYO*)malloc(1080*sizeof(FLOATHOKUYO));  //TODO: IL y a 1081 points à traiter!!!!
    data.depth = (uint32_t*)malloc(1080*sizeof(uint32_t));
    data.x_data = (FLOATHOKUYO*)malloc(1080*sizeof(FLOATHOKUYO));
    data.y_data = (FLOATHOKUYO*)malloc(1080*sizeof(FLOATHOKUYO));

    nb_scan = 0;
    stopSaveReplay();
    port_open = false;
    setVirtualSensor(); //par défaut, capteur virtuel
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHokuyoPlus::~CHokuyoPlus()

{
    free(data.angle);
    free(data.depth);
    free(data.x_data);
    free(data.y_data);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHokuyoPlus::setRealSensor()
{
    VirtualHokuyo = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHokuyoPlus::setVirtualSensor()
{
    VirtualHokuyo = 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHokuyoPlus::open_port()
{
    char chaineinit[1000];

    if ( (VirtualHokuyo == 0) && (port_open == false) ) {
        sprintf( chaineinit,"type=serial, device=%s, timeout=1", deviceName);
        string portOptions =  (string)chaineinit;

        startAngle = 0.0;
        endAngle = 0.0;
        firstStep = -1;
        lastStep = -1;
        baud = 19200;
	//baud = 500000;
        speed = 2400;
        clusterCount = 1;
        getIntensities = false;
        getNew = false;
        verbose = false;

        try
        {
            // Set the laser to verbose mode (    if (VirtualHokuyo==0)so we see more information in the console)
            if (verbose) laser.SetVerbose(true);

            try {
                laser.OpenWithProbing(portOptions);
                // Turn the laser on
                laser.SetPower(true);
            } catch(flexiport::PortException e) {
                printf("Exception: Failed to open device '%s'\n",deviceName);
                return;
            }

            hokuyo_aist::HokuyoSensorInfo info;
            laser.GetSensorInfo(&info);
            cout << info.AsString();
        }
        catch (hokuyo_aist::HokuyoError e)
        {
            cerr << "Caught exception: (" << e.Code () << ") " << e.what () << endl;
        }
        port_open = true;
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHokuyoPlus::close_port()
{
    if ( (VirtualHokuyo==0) && (port_open == true) )
    {
        laser.Close();
        port_open = false;
    }
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHokuyoPlus::setReplayFileName(string fileNameInit)
{
    fileName = fileNameInit;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHokuyoPlus::startSaveReplay()
{
    if (VirtualHokuyo == 0)
        saveReplay = 1;
    else
        saveReplay = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHokuyoPlus::stopSaveReplay()
{
    saveReplay=0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CHokuyoPlus::seekReplay(int new_nb_scan)
{
    nb_scan=new_nb_scan;
    if (nb_scan < 0) nb_scan=0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CHokuyoPlus::getRange()
{
            timespec  __tp;
            int toto = clock_gettime(CLOCK_REALTIME,&__tp);

            // Get range data
            hokuyo_aist::HokuyoData hokuyo_aistdata;
	    
            if (startAngle == 0.0 && endAngle == 0.0) {
                    laser.GetRanges(&hokuyo_aistdata, firstStep, lastStep, clusterCount);
            } else {
                    laser.GetRangesByAngle(&hokuyo_aistdata, startAngle, endAngle, clusterCount);
            }

            for (int i = 0 ; i <= lastStep-firstStep ; i++) {
                data.depth[i] = hokuyo_aistdata[i];
            }
	    
            hokuyotime = __tp.tv_sec + __tp.tv_nsec*1e-9;

	    printf("START%lf\n", hokuyotime);

    nb_scan++;
}

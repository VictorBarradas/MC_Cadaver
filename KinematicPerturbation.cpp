#include "KinematicPerturbation.h"

KinematicPerturbation::KinematicPerturbation(void)
{
	bReceivedTimeStamp = false;
	eventCounter = 0;
	isRecording = false;

	goalPosIndex = 0;
	goalPos[0] = 444;
	goalPos[1] = 580;
	movingSpeed[0] = 100;
	movingSpeed[1] = 1023;
	isPerturbing = false;
	isChangingSpeed = false;

//	isInitialized = dxl_initialize(DEFAULT_PORTNUM,DEFAULT_BAUDNUM);
	if(isInitialized) {
		hIOMutex = CreateMutex(NULL, FALSE, NULL);
		kill = 0;
		delayThread = 1;
		_beginthread(KinematicPerturbation::StaticRecordingLoop,0,this);
	}	
}

KinematicPerturbation::~KinematicPerturbation(void)
{
	if(isInitialized) {
		dxl_terminate();
		kill = 1;
		fclose(kinematicFile);
	}
}

void KinematicPerturbation::Perturb(void)
{
	isPerturbing = true;
	if(goalPosIndex == 0) {
		goalPosIndex = 1;
	}
	else {
		goalPosIndex = 0;
	}
}

void KinematicPerturbation::SineWavePerturbationLookUpTable(void)
{
	int amplitude = (goalPos[1] - goalPos[0])/2;
	int offset = 512;

	for(int i = 0; i < 25; i++) {
		sineWave[i] =  (int)(amplitude*sin(6.2839*i/25.0) + offset);
	}
	isSineWaveLookUp = true;
}

int KinematicPerturbation::SineWavePerturbationTimer(void)
{
	isSineWaveTimer = true;

	int amplitude = (goalPos[1] - goalPos[0])/2;
	int offset = 512;
	int sineWavePos;

	sineWavePos =  (int)(amplitude*sin(6.2839*4*timeData.getCurrentTime()) + offset);
	return sineWavePos;
}

void KinematicPerturbation::SendServoPosition(int position)
{
	dxl_write_word( DEFAULT_ID, P_GOAL_POSITION_L, position);
}

void KinematicPerturbation::ReadServoPosition(void)
{
	currentPos = dxl_read_word( DEFAULT_ID, P_PRESENT_POSITION_L );
}

void KinematicPerturbation::StopPerturbing(void)
{
	isSineWaveLookUp = false;
	isSineWaveTimer = false;
	isPerturbing = false;
}

void KinematicPerturbation::ChangeServoSpeed(void)
{
	isChangingSpeed = true;
	if(movingSpeedIndex == 0) {
		movingSpeedIndex = 1;
	}
	else {
		movingSpeedIndex = 0;
	}
}

void KinematicPerturbation::StaticRecordingLoop(void* a)
{
	((KinematicPerturbation*)a)->RecordingLoop();
}

void KinematicPerturbation::RecordingLoop(void)
{
	/*double tim = 0.0;
	time_t rawTime;
	struct tm *timeInfo;
	time(&rawtime);
	timeInfo = localtime(&rawTime);*/	
	int i = 0;

	kinematicFile = fopen("test.txt", "w");
	while(!kill) {	
		WaitForSingleObject(hIOMutex, INFINITE);
		if(isChangingSpeed) {
			dxl_write_word( DEFAULT_ID, P_MOVING_SPEED_L, movingSpeed[movingSpeedIndex]);
			isChangingSpeed = false;
		}
		if(isSineWaveLookUp) {
			if(i > 25) {
				i = 0;
			}
			dxl_write_word( DEFAULT_ID, P_GOAL_POSITION_L, sineWave[i]);
			i++;		
		}
		if(isSineWaveTimer)
		{
			SendServoPosition(SineWavePerturbationTimer());
		}
		if(isPerturbing) {
			dxl_write_word( DEFAULT_ID, P_GOAL_POSITION_L, goalPos[goalPosIndex]);
			isPerturbing = false;	
		}

		if(bReceivedTimeStamp) {
			if (eventCounter == 0) {
				dataFile = fopen(sFileName, "w");
				fprintf(
					dataFile,
					"time,pos\n"
				);
				eventCounter++;
			}
			else if(isRecording) {
				ReadServoPosition();
				fprintf(kinematicFile,"%f,%d\n",timeData.getCurrentTime(),currentPos);
			}
		}
		ReleaseMutex( hIOMutex);
	}
}

int KinematicPerturbation::startRecording()
{
	isRecording = true;
	return 0;
}

int KinematicPerturbation::stopRecording()
{
	isRecording = false;
	return 0;
}


int KinematicPerturbation::setTimeStamp(char *timeStamp)
{
	sprintf_s(
		sFileName, 
		"C:\\data\\%s_kinematic",
		timeStamp
	);

	bReceivedTimeStamp = true;

	return 0;
}
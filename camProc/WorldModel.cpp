// WorldModel Implementations

#include "WorldModel.h"
#include "ARCamera.h"
#include "ARMarker.h"

using namespace Eigen;
using namespace std;

// Constructor
WorldModel::WorldModel(vector<ARCamera> arcameras, vector<ARMarker> armarkers)
{
    cameras = arcameras;
    markers = armarkers;
}

bool WorldModel::setOrigin(int cameraID, int markerID, Matrix4d transform)
{
    // Initialize camera with World to Camera transform (given in input)
    bool success = cameras[getCamInd(cameraID)].initializeCamera(transform);

    // Set marker as "world" marker
    worldMarkerID = markerID;

    return success;
}

bool WorldModel::initCamera(int cam2InitID, int camAlreadyID, Matrix4d transMtoNew, Matrix4d transMtoOld)
{
    // Make sure camera is initialized
    if (!cameras[getCamInd(camAlreadyID)].isInitialized())
        return false;

    // Get world to camera transform for new camera
    Matrix4d world2new = cameras[getCamInd(camAlreadyID)].getWorld2Cam() * transMtoOld.inverse() * transMtoNew;

    // Initialize camera with that transform
    bool success = cameras[getCamInd(cam2InitID)].initializeCamera(world2new);

    return success;
}

bool WorldModel::setMarkerLoc(int cameraID, int markerID, Matrix4d transform)
{
    // Check that camera is initialized
    if (!cameras[getCamInd(cameraID)].isInitialized())
        return false;

    // Get world to marker matrix
    Matrix4d world2mark = cameras[getCamInd(cameraID)].getWorld2Cam() * transform.inverse();

    // Set marker
    bool success = markers[getMarkInd(markerID)].setMarker(world2mark);

    return success;
}

Vector3d WorldModel::getMarkerLoc(int markerID)
{
    return markers[getMarkInd(markerID)].getLocInWorld();
}

int WorldModel::getCamInd(int ID)
{
    for (int i = 0; i < cameras.size(); i++)
    {
        if (cameras[i].getID() == ID)
            return i;
    }
    
    // Return -1 if not in array
    return -1;
}

int WorldModel::getMarkInd(int ID)
{
    for (int i = 0; i < markers.size(); i++)
    {
        if (markers[i].getID() == ID)
            return i;
    }
    
    // Return -1 if not in array
    return -1;
}
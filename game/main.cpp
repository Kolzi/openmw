#include <iostream>

#include <string>
#include <fstream>

#include "boost/program_options.hpp"

#include "esm_store/cell_store.hpp"
#include "bsa/bsa_archive.hpp"
#include "ogre/renderer.hpp"
#include "tools/fileops.hpp"

#include "mwrender/cell.hpp"
#include "mwrender/mwscene.hpp"
#include "mwinput/inputmanager.hpp"

using namespace std;

void maintest (std::string dataDir)
{
  assert (!dataDir.empty());
  
  if (dataDir[dataDir.size()-1]!='/' && dataDir[dataDir.size()-1]!='\\')
    dataDir += "/";

  const char* esmFile = "Morrowind.esm";
  const char* bsaFile = "Morrowind.bsa";

  const char* plugCfg = "plugins.cfg";

  cout << "Hello, fellow traveler!\n";
  
  cout << "Your data directory for today is: " << dataDir << "\n";

  cout << "Initializing OGRE\n";
  Render::OgreRenderer ogre;
  ogre.configure(!isFile("ogre.cfg"), plugCfg, false);

  cout << "Adding " << bsaFile << endl;
  addBSA(dataDir + bsaFile);

  cout << "Loading ESM " << esmFile << "\n";
  ESM::ESMReader esm;
  ESMS::ESMStore store;
  ESMS::CellStore cell;

  // This parses the ESM file and loads a sample cell
  esm.open(dataDir + esmFile);
  store.load(esm);

  cell.loadInt("Beshara", store, esm);

  // Create the window
  ogre.createWindow("OpenMW");

  cout << "\nSetting up cell rendering\n";

  // Sets up camera, scene manager etc
  MWRender::MWScene scene(ogre);

  // This connects the cell data with the rendering scene.
  MWRender::CellRender rend(cell, scene);

  // Load the cell and insert it into the renderer
  rend.show();

  cout << "Setting up input system\n";

  // Sets up the input system
  MWInput::MWInputManager input(ogre);

  cout << "\nStart! Press Q/ESC or close window to exit.\n";

  // Start the main rendering loop
  ogre.start();

  cout << "\nThat's all for now!\n";
}

int main(int argc, char**argv)
{
  try
  {
    boost::program_options::options_description desc (
      "Syntax: openmw <options>\nAllowed options");

    desc.add_options()
      ("help", "print help message")
      ("data", boost::program_options::value<std::string>()->default_value ("data"),
        "set data directory"
      );
  
    boost::program_options::variables_map variables;
    
    std::ifstream configFile ("openmw.cfg");

    boost::program_options::store (
      boost::program_options::parse_command_line (argc, argv, desc), variables);
    boost::program_options::notify (variables);

    if (configFile.is_open())
      boost::program_options::store (
        boost::program_options::parse_config_file (configFile, desc), variables);

    if (variables.count ("help"))
    {
      std::cout << desc << std::endl;
    }
    else
    {          
      maintest (variables["data"].as<std::string>());
    }  
  }
  catch(exception &e)
    {
      cout << "\nERROR: " << e.what() << endl;
      return 1;
    }
  return 0;
}

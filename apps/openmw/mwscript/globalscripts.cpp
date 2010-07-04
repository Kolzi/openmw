
#include "globalscripts.hpp"

#include <cassert>

#include "interpretercontext.hpp"
#include "scriptmanager.hpp"

namespace MWScript
{
    GlobalScripts::GlobalScripts (const ESMS::ESMStore& store, ScriptManager& scriptManager)
    : mStore (store), mScriptManager (scriptManager)
    {
        addScript ("Main");
        
        for (ESMS::RecListT<ESM::StartScript>::MapType::const_iterator iter 
            (store.startScripts.list.begin()); 
            iter != store.startScripts.list.end(); ++iter)
            addScript (iter->second.script);
    }

    void GlobalScripts::addScript (const std::string& name)
    {
        if (mScripts.find (name)==mScripts.end())
            if (const ESM::Script *script = mStore.scripts.find (name))
            {           
                Locals locals;
                
                locals.configure (*script);

                mScripts.insert (std::make_pair (name, std::make_pair (true, locals))); 
            }
    }
    
    void GlobalScripts::removeScript (const std::string& name)
    {
        std::map<std::string, std::pair<bool, Locals> >::iterator iter = mScripts.find (name);

        if (iter!=mScripts.end())
            iter->second.first = false;
    }
        
    bool GlobalScripts::isRunning (const std::string& name) const
    {
        std::map<std::string, std::pair<bool, Locals> >::const_iterator iter =
            mScripts.find (name);
            
        if (iter==mScripts.end())
            return false;
            
        return iter->second.first;
    }
                
    void GlobalScripts::run (MWWorld::Environment& environment)
    {
        for (std::map<std::string, std::pair<bool, Locals> >::iterator iter (mScripts.begin());
            iter!=mScripts.end(); ++iter)
        {
            if (iter->second.first)
            {
                MWScript::InterpreterContext interpreterContext (environment,
                    &iter->second.second, MWWorld::Ptr());
                mScriptManager.run (iter->first, interpreterContext);        
            }
        }

    }
}


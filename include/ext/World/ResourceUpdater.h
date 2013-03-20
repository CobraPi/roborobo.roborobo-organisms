//
//  ResourceUpdater.h
//  roborobo
//
//  Created by Berend Weel on 3/18/13.
//
//

#ifndef roborobo_ResourceUpdater_h
#define roborobo_ResourceUpdater_h

/**
 * ResourceUpdater
 *
 * Abstract class (interface) for the classes that manage world resources, like ResourceFactory.
 *
 */
class ResourceUpdater {
public:
    virtual void step() = 0;
};

typedef boost::shared_ptr<ResourceUpdater> ResourceUpdaterPtr;

#endif

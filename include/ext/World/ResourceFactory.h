//
//  ResourceFactory.h
//  roborobo
//
//  Created by Berend Weel on 3/18/13.
//
//

#ifndef __roborobo__ResourceFactory__
#define __roborobo__ResourceFactory__

#include <iostream>
#include <vector>
#include <SDL/SDL.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "RoboroboMain/roborobo.h"
#include "Utilities/SDL_gfxPrimitives.h"
#include "Utilities/SDL_gfxPrimitivesExtra.h"
#include "World/ResourceUpdater.h"

template<class Resource>
class ResourceFactory : public ResourceUpdater {
public:
    typedef boost::shared_ptr<ResourceFactory<Resource> > ResourceFactoryPtr;
    typedef boost::shared_ptr<Resource> ResourcePtr;
    
    ResourceFactory(){
        nextId = 0;
        resources = std::vector<ResourcePtr>();
        
        // Prepare a surface to hold the energy spots so sensors can detect them
        resourceImage = SDL_CreateRGBSurface (gScreen->flags, gAreaWidth, gAreaHeight, gScreen->format->BitsPerPixel, gScreen->format->Rmask, gScreen->format->Gmask, gScreen->format->Bmask, gScreen->format->Amask);
        SDL_SetColorKey(resourceImage, SDL_SRCCOLORKEY, SDL_MapRGB(resourceImage->format, 0xFF, 0xFF, 0xFF));
        SDL_FillRect(resourceImage, NULL, SDL_MapRGB(resourceImage->format,0xFF, 0xFF, 0xFF));
    }
    
    virtual ~ResourceFactory(){
        SDL_FreeSurface(resourceImage);
        resources.clear();
    }
    
    static ResourceFactoryPtr getInstance(){
        if(!_instance){
            _instance = boost::make_shared<ResourceFactory<Resource> >();
        }
        return _instance;
    }
    
    virtual void step(){
        for (typename std::vector<ResourcePtr>::iterator it = resources.begin(); it != resources.end(); it++){
            (*it)->step(resourceImage);
        }
    }
    
    virtual void displayResources(){
        for (typename std::vector<ResourcePtr>::iterator it = resources.begin(); it != resources.end(); it++){
            (*it)->display(resourceImage);
        }
    }
    
    virtual ResourcePtr addResource(){
        boost::shared_ptr<Resource> resource = boost::make_shared<Resource>(nextId++,resourceImage);
        resources.push_back(resource);
        return resource;
    }
    
    virtual void removeResource(ResourcePtr resource){
        resources.erase(std::find(resources.begin(),resources.end(),resource));
    }

    virtual std::vector<ResourcePtr> getResources(){
        return resources;
    }

    virtual void clear(){
        resources.clear();
        nextId = 0;
    }

    virtual SDL_Surface* getSDLSurface(){
        return resourceImage;
    }    
    
    virtual int castRay(double x1, double y1, double *x2pt, double *y2pt, int __maxValue ){
        return castSensorRay(resourceImage,x1,y1,x2pt,y2pt,__maxValue);
    }

    virtual Uint32 getPixel(int x, int y){
        return getPixel32(resourceImage,x,y);
    }

    virtual void getRGB(int x, int y, Uint8 *r, Uint8 *g, Uint8 *b){
        Uint32 pixel = getPixel(x,y);
        SDL_GetRGB(pixel, resourceImage->format, r, g, b);
    }

    
private:
    static ResourceFactoryPtr _instance;
    
    SDL_Surface *resourceImage;
    std::vector<ResourcePtr> resources;
    int nextId;
};

template <class Resource>
typename ResourceFactory<Resource>::ResourceFactoryPtr ResourceFactory<Resource>::_instance = boost::shared_ptr<ResourceFactory<Resource> >();

#endif /* defined(__roborobo__ResourceFactory__) */

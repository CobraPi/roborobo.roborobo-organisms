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
#include "Utilities/SDL_gfxRoborobo.h"
#include "World/ResourceUpdater.h"

/**
 * \class ResourceFactory
 *
 * \brief ResourceFactory is a singleton class that manages resources in the world \sa Resource
 *
 * The class maintains a vector of resources, as well as an image to cast sensors on
 * Resources should be created by getting the ResourceFactory instance and using addResource.
 *
 * getSDLSurface, castRay, getPixel, and getRBG are meant for implementations of Resource
 *
 * step is mean for the World class.
 */
template<class Resource>
class ResourceFactory : public ResourceUpdater {
public:
    typedef boost::shared_ptr<ResourceFactory<Resource> > ResourceFactoryPtr; ///< Boost shared_ptr to ResourceFactory
    typedef boost::shared_ptr<Resource> ResourcePtr; ///< Boost shared_ptr to a Resource

    /**
     * Default constructor
     *
     * Initializes an empty resource vector and image
     *
     */
    ResourceFactory(){
        nextId = 0;
        resources = std::vector<ResourcePtr>();
        
        // Prepare a surface to hold the resources so sensors can detect them
        resourceImage = SDL_CreateRGBSurface (gScreen->flags, gAreaWidth, gAreaHeight, gScreen->format->BitsPerPixel, gScreen->format->Rmask, gScreen->format->Gmask, gScreen->format->Bmask, gScreen->format->Amask);
        
        // White is transparant
        SDL_SetColorKey(resourceImage, SDL_SRCCOLORKEY, SDL_MapRGB(resourceImage->format, 0xFF, 0xFF, 0xFF));
        
        // Fill the image with white
        SDL_FillRect(resourceImage, NULL, SDL_MapRGB(resourceImage->format,0xFF, 0xFF, 0xFF));
    }
    
    /**
     * Default destructor
     */
    virtual ~ResourceFactory(){
        SDL_FreeSurface(resourceImage);
        resources.clear();
    }
    
    /**
     * Get the ResourceFactory instance
     *
     * @return the ResourceFactory
     */
    static ResourceFactoryPtr getInstance(){
        if(!_instance){
            _instance = boost::make_shared<ResourceFactory<Resource> >();
        }
        return _instance;
    }
    
    /**
     * Calls step on all resources managed by this factory
     */
    virtual void step(){
        for (typename std::vector<ResourcePtr>::iterator it = resources.begin(); it != resources.end(); it++){
            (*it)->step(resourceImage);
            if(!(*it)->getActiveStatus()){
                (*it)->hide(resourceImage);
            }
        }
    }
    
    /*!
     * Calls display on all resources managed by this factory
     */
    virtual void displayResources(){
        for (typename std::vector<ResourcePtr>::iterator it = resources.begin(); it != resources.end(); it++){
            if((*it)->getActiveStatus()){
                (*it)->display(resourceImage);
            }
        }
    }
    
    /**
     * Creates and adds a new resource
     *
     * @return The resource
     */
    virtual ResourcePtr addResource(){
        boost::shared_ptr<Resource> resource = boost::make_shared<Resource>(nextId++,resourceImage);
        resources.push_back(resource);
        return resource;
    }
    
    /**
     * Removes a resource
     *
     * @param resource The resource
     */
    virtual void removeResource(ResourcePtr resource){
        resources.erase(std::find(resources.begin(),resources.end(),resource));
    }
    
    virtual void setActiveStatus(ResourcePtr resource,bool status) {
        resource->setActiveStatus(false);
        resource->hide(resourceImage);
    }

    /**
     * Returns a vector of all resources managed by this factory
     */
    virtual std::vector<ResourcePtr> getResources(){
        return resources;
    }

    /**
     * Clears all resources managed by this factory
     */
    virtual void clear(){
        resources.clear();
        nextId = 0;
        SDL_FillRect(resourceImage, NULL, SDL_MapRGB(resourceImage->format,0xFF, 0xFF, 0xFF));
    }

    /**
     * Returns the SDL_Surface the resources are displayed on
     *
     * @return the vector
     */
    virtual SDL_Surface* getSDLSurface(){
        return resourceImage;
    }    
    
    /**
     * Casts a ray on the SDL_Surface from x1,y1 to x2pt,y2pt
     *
     * @param x1 Origin x-coordinate
     * @param y1 Origin y-coordinate
     * @param x2pt Target x-coordinate, updated to the actual sensed value
     * @param y2pt Target y-coordiante, updated to the actual sensed value
     * @param __maxValue Maximum distance of the ray
     *
     * @return The distance at which the ray encountered something
     */
    virtual int castRay(double x1, double y1, double *x2pt, double *y2pt, int __maxValue ){
        return castSensorRay(resourceImage,x1,y1,x2pt,y2pt,__maxValue);
    }

    /**
     * Returns the value of the pixel at position x,y on the manage SDL_Surface
     *
     * @param x x-coordinate
     * @param y y-coordinate
     *
     * @return The value of the pixel
     */
    virtual Uint32 getPixel(int x, int y){
        return getPixel32(resourceImage,x,y);
    }

    /**
     * Sets r,g and b to the red, green and blue values at position x,y
     *
     * @param x x-coordinate
     * @param y y-coordiante
     * @param r destination for red value
     * @param g destination for green value
     * @param b destination for blue value
     */
    virtual void getRGB(int x, int y, Uint8 *r, Uint8 *g, Uint8 *b){
        Uint32 pixel = getPixel(x,y);
        SDL_GetRGB(pixel, resourceImage->format, r, g, b);
    }
    
private:
    static ResourceFactoryPtr _instance; ///< The resource factory instance
    
    SDL_Surface *resourceImage; ///< SDL_Surface to manage the resources and cast sensor rays on
    std::vector<ResourcePtr> resources; ///< Vector of the managed resources
    int nextId; ///< Id for the next created Resource
};

template <class Resource>
typename ResourceFactory<Resource>::ResourceFactoryPtr ResourceFactory<Resource>::_instance = boost::shared_ptr<ResourceFactory<Resource> >();

#endif /* defined(__roborobo__ResourceFactory__) */

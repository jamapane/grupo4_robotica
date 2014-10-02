/*
 *    Copyright (C) 2006-2010 by RoboLab - University of Extremadura
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */
 
 #include "specificworker.h"
#include <boost/concept_check.hpp>

/**
* \brief Default constructor
*/

SpecificWorker::SpecificWorker(MapPrx& mprx, QObject *parent) : GenericWorker(mprx, parent)	
{
	  temporizador.start();
}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{

}
void SpecificWorker::compute( )
{
  TLaserData laser_data = laser_proxy->getLaserData();

  int tiempoGirando;
  int velocidad;
  
 
  bool avanzando = true;
 
  try{
	
	switch(S){
		case STATE::PREPARARAVANZAR:	
			prepararAvanzar();
			break;
		case STATE::AVANZAR:	
			avanzar(laser_data);
			break;
			
		case STATE::PARAR:
			parar();
			break;
			
		case STATE::DIRECCIONGIRAR:
			velocidad = velocidadGiro();
			
		case STATE::PREPARARGIRAR:		
			tiempoGirando = tiempoGiro();
			
			break;

		case STATE::GIRAR:
             accionGirar(tiempoGirando, velocidad,laser_data);
			break;  
			
			
		
	}
	
  }
  catch(const Ice::Exception &e)
  {
	  std::cout <<e<<std::endl;
  }
 

  
}

bool SpecificWorker::comprobarChoque(TLaserData laser_data){
	 for (auto i:laser_data){
			if (i.dist< 500 and i.angle < 1.3 and i.angle > -1.3)
				return true;
	 }
	return false;	
}

int SpecificWorker::tiempoGiro(){
	temporizador.restart();
	int tiempoGiro = (qrand() % ((700 + 1) - 100) + 100);
	S=STATE::GIRAR;
	return tiempoGiro;
}

int SpecificWorker::velocidadGiro(){
	int	velocidadGiro = qrand() % ((1 + 1) - 0) + 0;
		if(velocidadGiro == 0)
			velocidadGiro = -1;
		S=STATE::PREPARARGIRAR;
		return velocidadGiro;
}

void SpecificWorker::parar(){
	 differentialrobot_proxy->stopBase();
	 S = STATE::DIRECCIONGIRAR;
}
void SpecificWorker::avanzar(TLaserData laser_data){			
	if(comprobarChoque(laser_data)){
		S=STATE::PARAR;
	}
	else
		differentialrobot_proxy->setSpeedBase(300,0);
}

void SpecificWorker::accionGirar(int tiempoGiro, int velocidad,TLaserData laser_data){
	if(temporizador.elapsed() < tiempoGiro){ //100-700
		differentialrobot_proxy->setSpeedBase(0,velocidad);	
	}
	else if (comprobarChoque(laser_data)){
		S=STATE::PREPARARGIRAR;
	}
			
	else{ 
        S=STATE::PREPARARAVANZAR;
	}
}

void SpecificWorker::prepararAvanzar(){
	differentialrobot_proxy->stopBase();
	S=STATE::AVANZAR;
}
bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
	timer.start(Period);
	return true;
};

void  SpecificWorker::newAprilTag(const tagsList& tags){
}
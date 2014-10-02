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

  int tiempoGiro;
  int velocidadGiro;
  
 
  bool avanzando = true;
 
  try{
	
	switch(S){
		case STATE::AVANZAR:	
			if(comprobarChoque(laser_data)){
				S=STATE::PARAR;
				cout << "Estoy avanzando y me choco" << endl;
			}
			else
				avanzar();
			
			break;
			
		case STATE::PARAR:
			cout << "Reinicio reloj" << endl;
			
			differentialrobot_proxy->stopBase();
			S = STATE::DIRECCIONGIRAR;
			cout << "Me he chocado y me paro" << endl;
			break;
			
		case STATE::DIRECCIONGIRAR:
			velocidadGiro = qrand() % ((1 + 1) - 0) + 0;
			if(velocidadGiro == 0)
				velocidadGiro = -1;
			S=STATE::PREPARARGIRAR;
			
			cout<<"Calculo velocidad de Giro: "<<velocidadGiro<<endl;
			break;
			
		case STATE::PREPARARGIRAR:		
			temporizador.restart();
			tiempoGiro = (qrand() % ((700 + 1) - 100) + 100);
			cout<<" Calculo tiempo de giro: "<<tiempoGiro<<endl;
			S=STATE::GIRAR;
			
			break;

		case STATE::GIRAR:
			cout << "Estoy girando" << endl;
			if(temporizador.elapsed() < tiempoGiro){ //100-700
				differentialrobot_proxy->setSpeedBase(0,velocidadGiro);
				cout <<"Tiempo:" << temporizador.elapsed() << endl;
				
				
			}
			else if (comprobarChoque(laser_data)){
				differentialrobot_proxy->stopBase();	
				S=STATE::PREPARARGIRAR;
					
			}
			
			else{ 
				differentialrobot_proxy->stopBase();
				S=STATE::AVANZAR;
			}
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

void SpecificWorker::rotar(){
		
}

void SpecificWorker::avanzar(){
	differentialrobot_proxy->setSpeedBase(300,0);
}



bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
	timer.start(Period);
	return true;
};

void  SpecificWorker::newAprilTag(const tagsList& tags){
}
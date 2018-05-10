/**
 * ImageFigureInterface.h
 *
 *  Created on: 17.05.2016
 *      Author: Taborko Maria
 */

#ifndef IMAGEFIGUREINTERFACE_H_
#define IMAGEFIGUREINTERFACE_H_

class ImageFigureInterface
{
	public:

		/**
		*	Конструктор.
		*/
		ImageFigureInterface(){}

		/**
		*	Деструктор.
		*/
		virtual ~ImageFigureInterface(){}
};

/**
 * Декларация интерфейса
 */
Q_DECLARE_INTERFACE( ImageFigureInterface, "com.alxn1.MultiFusion.TimelineInterface/1.0" )


/**
*	Приводит указатель на любой обьект к указателью на интерфейс TimelineInterface.
*
*	@return Приведённый указатель или 0, если произошла ошибка.
*/
template < class T > inline ImageFigureInterface* UNDOREDO( T *o )
{
	return qobject_cast< ImageFigureInterface* >( o );
}
#endif /* IMAGEFIGUREINTERFACE_H_ */

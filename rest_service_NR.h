/**
 * File              : rest_service_NR.h
 * Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
 * Date              : 12.10.2021
 * Last Modified Date: 18.10.2021
 * Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>
 */
#include <stdio.h>
#include "cJSON.h"
#include <stdbool.h>

//******************************************************************************************************
//Сервис получения внутренних справочников
struct	egisz_rest_refs_list_struct {
	int id;
	char refsName[BUFSIZ];	
};

int egisz_rest_refs_list(struct egisz_rest_refs_list_struct **array); //Получение списка справочников
//возвращает количество записей массива array, -1 при ошибке

int egisz_rest_refs_parts //Возвращает количество частей справочника, которое можно получить
( 
		int id //Идентификатор справочника
); 

struct	egisz_rest_refs_part_struct {
	int id;
	char region[256];	
	int territoryCode;
	bool isFederalCity;
};

int egisz_rest_refs_part
//Возвращает записи справочника по его идентификатору
//возвращает количество записей массива array, -1 при ошибке
(
		struct egisz_rest_refs_part_struct **array, //указатель на возвращаемый массив 
		int id,  //Идентификатор справочника 
		int part //Часть записей справочника, которую необходимо получить
); 
//******************************************************************************************************

//******************************************************************************************************
//Сервис работы с организациями
cJSON *egisz_rest_org_part //Возвращает часть записей организаций по типу
(
		int orgTypeId, //Тип организации 
		int part //Часть записей организаций, которую необходимо получить
); 

cJSON *egisz_rest_org_parts //Возвращает количество частей записей организаций по типу
( 
		int orgTypeId //Тип организации 
); 

cJSON *egisz_rest_org_get //Возвращает базовые сведения об организации по ее идентификатору (oid)
(
		char *oid //Идентификатор организации
); 
//******************************************************************************************************


//******************************************************************************************************
//Сервис работы с подразделениями медицинских организаций
cJSON *egisz_rest_depart_parts //Возвращает количество частей записей подразделений по типу
(
		int departTypeId //Тип подразделения
); 

cJSON *egisz_rest_depart_part //Возвращает часть записей подразделений по типу
(
		int departTypeId, //Тип подразделения 
		int part //Часть записей подразделений, которую необходимо получить
); 

cJSON *egisz_rest_depart_get //Возвращает сведения о подразделении по идентификатору (oid)
(
		char *oid //Идентификатор подразделения (oid)
); 
//******************************************************************************************************


//******************************************************************************************************
//Сервис работы с медицинскими работниками

//******************************************************************************************************


//******************************************************************************************************
//Сервис публикации сведений о медицинской организации на внешних информационных ресурсах
cJSON *egisz_rest_mo //Возвращает сведения о подразделении по идентификатору (oid)
(
		char *oid //Идентификатор организации (oid)
); 

//******************************************************************************************************


//******************************************************************************************************
//Сервис передачи МО по параметрам Вида работ и лицензий
cJSON *egisz_rest_lic_get //Возвращает сведения о подразделении по идентификатору (oid)
(
		int service //Идентификатор вида услуг/работ (service)
); 

//******************************************************************************************************

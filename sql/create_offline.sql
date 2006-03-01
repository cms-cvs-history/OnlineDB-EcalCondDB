CREATE TABLE CMS_ECAL."POOL_RSS_DB" ("GUID" CHAR(36) NOT NULL, "NUMBER_OF_CONTAINERS" NUMBER(10) NOT NULL );

-- create the table holding the container headers
CREATE TABLE CMS_ECAL."POOL_RSS_CONTAINERS" ("CONTAINER_ID" NUMBER(10) NOT NULL,"CONTAINER_NAME" VARCHAR2(4000) NOT NULL UNIQUE,  "CONTAINER_TYPE" VARCHAR2(4000) NOT NULL, "TABLE_NAME" VARCHAR2(4000), "CLASS_NAME" VARCHAR2(4000), "MAPPING_VERSION" VARCHAR2(4000), "NUMBER_OF_WRITTEN_OBJECTS" NUMBER(10) NOT NULL,  "NUMBER_OF_DELETED_OBJECTS" NUMBER(10) NOT NULL, CONSTRAINT "POOL_RSS_CONTAINERS_PK" PRIMARY KEY("CONTAINER_ID") );

-- create primitive container tables
CREATE TABLE CMS_ECAL."POOL_CONT___LINKS" ("OID" NUMBER(10) NOT NULL,  "VALUE" VARCHAR2(4000) NOT NULL);
CREATE TABLE CMS_ECAL."POOL_CONT___PARAMS" ("OID" NUMBER(10) NOT NULL,  "VALUE" VARCHAR2(4000) NOT NULL);
CREATE TABLE CMS_ECAL."POOL_CONT___SHAPES" ("OID" NUMBER(10) NOT NULL,  "VALUE" VARCHAR2(4000) NOT NULL);

-- Create POOL tables
CREATE TABLE CMS_ECAL."POOL_OR_MAPPING_VERSIONS" ( "MAPPING_VERSION" VARCHAR2(4000) NOT NULL UNIQUE );
CREATE TABLE CMS_ECAL."POOL_OR_MAPPING_ELEMENTS" ( "ELEMENT_TYPE" VARCHAR2(4000) NOT NULL, "SCOPE_NAME" VARCHAR2(4000) NOT NULL, "VARIABLE_NAME" VARCHAR2(4000) NOT NULL, "VARIABLE_TYPE" VARCHAR2(4000) NOT NULL, "TABLE_NAME" VARCHAR2(4000) NOT NULL, "VERSION" VARCHAR2(4000) NOT NULL );
CREATE TABLE CMS_ECAL."POOL_OR_MAPPING_COLUMNS" ( "VERSION" VARCHAR2(4000) NOT NULL, "SCOPE_NAME" VARCHAR2(4000) NOT NULL, "VARIABLE_NAME" VARCHAR2(4000) NOT NULL, "COLUMN_NAME" VARCHAR2(4000) NOT NULL, "POSITION" NUMBER(10) NOT NULL );

-- Create payload object tables generated from EcalPedestals-mapping-custom.xml
CREATE TABLE CMS_ECAL."ECALPEDESTALS" ( "IOV_VALUE_ID" NUMBER(10), CONSTRAINT "ECALPEDESTALS_PK" PRIMARY KEY ( "IOV_VALUE_ID" ) );
CREATE TABLE CMS_ECAL."ECALPEDESTALS_ITEM" ( "POS" NUMBER(10) NOT NULL, "IOV_VALUE_ID" NUMBER(10) NOT NULL, "MEAN_X1" BINARY_FLOAT NOT NULL, "MEAN_X12" BINARY_FLOAT NOT NULL, "MEAN_X6" BINARY_FLOAT NOT NULL, "RMS_X1" BINARY_FLOAT NOT NULL, "RMS_X12" BINARY_FLOAT NOT NULL, "RMS_X6" BINARY_FLOAT NOT NULL, "DET_ID" NUMBER(10) NOT NULL, CONSTRAINT "ECALPEDESTALS_ITEM_PK" PRIMARY KEY ( "IOV_VALUE_ID","POS" ), CONSTRAINT "ECALPEDESTALS_ITEM_ID_FK" FOREIGN KEY ( "IOV_VALUE_ID" ) REFERENCES CMS_ECAL."ECALPEDESTALS" ( "IOV_VALUE_ID" ) );

-- Add TIME column for IOV
ALTER TABLE CMS_ECAL."ECALPEDESTALS" ADD "TIME" NUMBER(10);

-- Create payload object tables generated from ./EcalWeight-mapping-custom.xml
CREATE TABLE CMS_ECAL."ECALWEIGHT" ( "IOV_VALUE_ID" NUMBER(10), "WEIGHT" BINARY_DOUBLE NOT NULL, CONSTRAINT "ECALWEIGHT_PK" PRIMARY KEY ( "IOV_VALUE_ID" ) );

-- Add TIME column for IOV
ALTER TABLE CMS_ECAL."ECALWEIGHT" ADD "TIME" NUMBER(10);

-- Create payload object tables generated from ./EcalWeightRecAlgoWeights-mapping-custom.xml
CREATE TABLE CMS_ECAL."ECALWEIGHTRECALGOWEIGHTS" ( "IOV_VALUE_ID" NUMBER(10), CONSTRAINT "ECALWEIGHTRECALGOWEIGHTS_PK" PRIMARY KEY ( "IOV_VALUE_ID" ) );
CREATE TABLE CMS_ECAL."WGTAFTERSWITCH" ( "POS" NUMBER(10) NOT NULL, "ID_ID" NUMBER(10) NOT NULL, CONSTRAINT "WGTAFTERSWITCH_PK" PRIMARY KEY ( "ID_ID","POS" ), CONSTRAINT "WGTAFTERSWITCH_ID_FK" FOREIGN KEY ( "ID_ID" ) REFERENCES CMS_ECAL."ECALWEIGHTRECALGOWEIGHTS" ( "IOV_VALUE_ID" ) );
CREATE TABLE CMS_ECAL."WGTAFTERSWITCH_VEC" ( "POS" NUMBER(10) NOT NULL, "ID_POS" NUMBER(10) NOT NULL, "ID_ID_ID" NUMBER(10) NOT NULL, "WEIGHT" BINARY_DOUBLE NOT NULL, CONSTRAINT "WGTAFTERSWITCH_VEC_PK" PRIMARY KEY ( "ID_ID_ID","ID_POS","POS" ), CONSTRAINT "WGTAFTERSWITCH_VEC_ID_FK" FOREIGN KEY ( "ID_ID_ID","ID_POS" ) REFERENCES CMS_ECAL."WGTAFTERSWITCH" ( "ID_ID","POS" ) );
CREATE TABLE CMS_ECAL."WGTBEFORESWITCH" ( "POS" NUMBER(10) NOT NULL, "ID_ID" NUMBER(10) NOT NULL, CONSTRAINT "WGTBEFORESWITCH_PK" PRIMARY KEY ( "ID_ID","POS" ), CONSTRAINT "WGTBEFORESWITCH_ID_FK" FOREIGN KEY ( "ID_ID" ) REFERENCES CMS_ECAL."ECALWEIGHTRECALGOWEIGHTS" ( "IOV_VALUE_ID" ) );
CREATE TABLE CMS_ECAL."WGTBEFORESWITCH_VEC" ( "POS" NUMBER(10) NOT NULL, "ID_POS" NUMBER(10) NOT NULL, "ID_ID_ID" NUMBER(10) NOT NULL, "WEIGHT" BINARY_DOUBLE NOT NULL, CONSTRAINT "WGTBEFORESWITCH_VEC_PK" PRIMARY KEY ( "ID_ID_ID","ID_POS","POS" ), CONSTRAINT "WGTBEFORESWITCH_VEC_ID_FK" FOREIGN KEY ( "ID_ID_ID","ID_POS" ) REFERENCES CMS_ECAL."WGTBEFORESWITCH" ( "ID_ID","POS" ) );
CREATE TABLE CMS_ECAL."WGTCHI2AFTERSWITCH" ( "POS" NUMBER(10) NOT NULL, "ID_ID" NUMBER(10) NOT NULL, CONSTRAINT "WGTCHI2AFTERSWITCH_PK" PRIMARY KEY ( "ID_ID","POS" ), CONSTRAINT "WGTCHI2AFTERSWITCH_ID_FK" FOREIGN KEY ( "ID_ID" ) REFERENCES CMS_ECAL."ECALWEIGHTRECALGOWEIGHTS" ( "IOV_VALUE_ID" ) );
CREATE TABLE CMS_ECAL."WGTCHI2AFTERSWITCH_VEC" ( "POS" NUMBER(10) NOT NULL, "ID_POS" NUMBER(10) NOT NULL, "ID_ID_ID" NUMBER(10) NOT NULL, "WEIGHT" BINARY_DOUBLE NOT NULL, CONSTRAINT "WGTCHI2AFTERSWITCH_VEC_PK" PRIMARY KEY ( "ID_ID_ID","ID_POS","POS" ), CONSTRAINT "WGTCHI2AFTERSWITCH_VEC_ID_FK" FOREIGN KEY ( "ID_ID_ID","ID_POS" ) REFERENCES CMS_ECAL."WGTCHI2AFTERSWITCH" ( "ID_ID","POS" ) );
CREATE TABLE CMS_ECAL."WGTCHI2BEFORESWITCH" ( "POS" NUMBER(10) NOT NULL, "ID_ID" NUMBER(10) NOT NULL, CONSTRAINT "WGTCHI2BEFORESWITCH_PK" PRIMARY KEY ( "ID_ID","POS" ), CONSTRAINT "WGTCHI2BEFORESWITCH_ID_FK" FOREIGN KEY ( "ID_ID" ) REFERENCES CMS_ECAL."ECALWEIGHTRECALGOWEIGHTS" ( "IOV_VALUE_ID" ) );
CREATE TABLE CMS_ECAL."WGTCHI2BEFORESWITCH_VEC" ( "POS" NUMBER(10) NOT NULL, "ID_POS" NUMBER(10) NOT NULL, "ID_ID_ID" NUMBER(10) NOT NULL, "WEIGHT" BINARY_DOUBLE NOT NULL, CONSTRAINT "WGTCHI2BEFORESWITCH_VEC_PK" PRIMARY KEY ( "ID_ID_ID","ID_POS","POS" ), CONSTRAINT "WGTCHI2BEFORESWITCH_VEC_ID_FK" FOREIGN KEY ( "ID_ID_ID","ID_POS" ) REFERENCES CMS_ECAL."WGTCHI2BEFORESWITCH" ( "ID_ID","POS" ) );

-- Add TIME column for IOV
ALTER TABLE CMS_ECAL."ECALWEIGHTRECALGOWEIGHTS" ADD "TIME" NUMBER(10);

-- Create IOV and MetaData table
CREATE TABLE CMS_ECAL."COND__IOV" ( "ID" NUMBER(10), CONSTRAINT "COND__IOV_PK" PRIMARY KEY ( "ID" ) );
CREATE TABLE CMS_ECAL."COND__IOV_IOV" ( "POS" NUMBER(10) NOT NULL, "ID_ID" NUMBER(10) NOT NULL, "IOV_IOV_STRING" VARCHAR2(4000) NOT NULL, "IOV_IOV_UNSIGNED_LONG" NUMBER(10) NOT NULL, CONSTRAINT "COND__IOV_IOV_PK" PRIMARY KEY ( "ID_ID","POS" ), CONSTRAINT "COND__IOV_IOV_ID_FK" FOREIGN KEY ( "ID_ID" ) REFERENCES CMS_ECAL."COND__IOV" ( "ID" ) );
CREATE TABLE CMS_ECAL."METADATA" ( "NAME" VARCHAR2(4000), "TOKEN" VARCHAR2(4000) NOT NULL, CONSTRAINT "METADATA_PK" PRIMARY KEY ( "NAME" ) );

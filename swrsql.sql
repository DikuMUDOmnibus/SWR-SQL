-- MySQL dump 10.10
--
-- Host: localhost    Database: swrsql
-- ------------------------------------------------------
-- Server version	5.0.24a-Debian_9-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `boards`
--

DROP TABLE IF EXISTS `boards`;
CREATE TABLE `boards` (
  `id` int(3) NOT NULL auto_increment,
  `read_group` longtext,
  `post_group` longtext,
  `extra_readers` longtext,
  `extra_removers` longtext,
  `board_obj` int(15) default NULL,
  `min_read_level` int(4) default NULL,
  `min_post_level` int(4) default NULL,
  `min_remove_level` int(4) default NULL,
  `max_posts` int(10) default NULL,
  `type` int(4) default NULL,
  `name` varchar(100) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=13 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `boards`
--


/*!40000 ALTER TABLE `boards` DISABLE KEYS */;
LOCK TABLES `boards` WRITE;
INSERT INTO `boards` VALUES (1,'','','','',10414,0,0,0,100,0,'student'),(2,NULL,NULL,NULL,NULL,100,101,101,102,100,0,'imm_monitor'),(3,NULL,NULL,NULL,NULL,102,101,101,103,30,0,'quest'),(4,NULL,NULL,NULL,NULL,32201,0,0,103,100,0,'galactic_net'),(5,NULL,NULL,NULL,NULL,38,0,0,0,200,1,'mail_net'),(6,'the new republic','The New republic',NULL,NULL,10100,102,102,103,50,0,'nr'),(7,'The Empire','The Empire',NULL,NULL,10199,102,102,103,50,0,'imperial'),(8,NULL,NULL,NULL,NULL,0,0,0,0,0,0,'idea'),(9,NULL,NULL,NULL,NULL,7312,1,1,101,20,0,'bidea'),(10,NULL,NULL,NULL,NULL,21500,2,2,100,25,0,'ackbuild'),(11,NULL,NULL,NULL,NULL,21435,2,100,101,50,0,'blueprint'),(12,'the order of jedis','the order of jedis',NULL,NULL,6199,101,101,101,50,0,'jedi');
UNLOCK TABLES;
/*!40000 ALTER TABLE `boards` ENABLE KEYS */;

--
-- Table structure for table `bounties`
--

DROP TABLE IF EXISTS `bounties`;
CREATE TABLE `bounties` (
  `id` int(3) NOT NULL auto_increment,
  `price` int(50) default NULL,
  `bounty` varchar(100) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `bounties`
--


/*!40000 ALTER TABLE `bounties` DISABLE KEYS */;
LOCK TABLES `bounties` WRITE;
INSERT INTO `bounties` VALUES (1,1000000,'Frank'),(2,666,'Your Mom'),(4,4032493,'Admin');
UNLOCK TABLES;
/*!40000 ALTER TABLE `bounties` ENABLE KEYS */;

--
-- Table structure for table `clans`
--

DROP TABLE IF EXISTS `clans`;
CREATE TABLE `clans` (
  `id` int(3) NOT NULL auto_increment,
  `name` varchar(100) default NULL,
  `description` longtext,
  `leader` varchar(100) default NULL,
  `number1` varchar(100) default NULL,
  `number2` varchar(100) default NULL,
  `type` int(3) default NULL,
  `board` int(15) default NULL,
  `storeroom` int(15) default NULL,
  `guard1` int(15) default NULL,
  `guard2` int(15) default NULL,
  `patrol1` int(15) default NULL,
  `patrol2` int(15) default NULL,
  `trooper1` int(15) default NULL,
  `trooper2` int(15) default NULL,
  `funds` bigint(50) default NULL,
  `jail` int(15) default NULL,
  `temp` varchar(255) default NULL,
  `mkills` int(3) default NULL,
  `typename` varchar(100) default NULL,
  `mainclan` int(3) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `clans`
--


/*!40000 ALTER TABLE `clans` DISABLE KEYS */;
LOCK TABLES `clans` WRITE;
UNLOCK TABLES;
/*!40000 ALTER TABLE `clans` ENABLE KEYS */;

--
-- Table structure for table `notes`
--

DROP TABLE IF EXISTS `notes`;
CREATE TABLE `notes` (
  `sender` varchar(100) default NULL,
  `date` varchar(100) default NULL,
  `to_list` longtext,
  `subject` varchar(100) default NULL,
  `voting` int(4) default NULL,
  `id` int(3) NOT NULL auto_increment,
  `boardid` int(3) default NULL,
  `text` longtext,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `notes`
--


/*!40000 ALTER TABLE `notes` DISABLE KEYS */;
LOCK TABLES `notes` WRITE;
INSERT INTO `notes` VALUES ('Admin','Tue Jan  3 11:38:45 2006','Your Nom','TURDIO',1,3,2,'e3j892rh8 h2q8r fh8e4i hr82h r832h\n\nr832hr\n\nturd\n\n');
UNLOCK TABLES;
/*!40000 ALTER TABLE `notes` ENABLE KEYS */;

--
-- Table structure for table `planets`
--

DROP TABLE IF EXISTS `planets`;
CREATE TABLE `planets` (
  `id` int(3) NOT NULL auto_increment,
  `starsystem` int(3) default NULL,
  `areas` longtext,
  `name` varchar(100) default NULL,
  `base_value` bigint(50) default NULL,
  `governed_by` int(3) default NULL,
  `population` bigint(50) default NULL,
  `flags` bigint(50) default NULL,
  `pop_support` float default NULL,
  `x` int(10) default NULL,
  `y` int(10) default NULL,
  `z` int(10) default NULL,
  `type` int(3) default NULL,
  `gravity` int(10) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `planets`
--


/*!40000 ALTER TABLE `planets` DISABLE KEYS */;
LOCK TABLES `planets` WRITE;
UNLOCK TABLES;
/*!40000 ALTER TABLE `planets` ENABLE KEYS */;

--
-- Table structure for table `ships`
--

DROP TABLE IF EXISTS `ships`;
CREATE TABLE `ships` (
  `id` int(5) NOT NULL auto_increment,
  `starsystem` int(5) default NULL,
  `name` varchar(255) default NULL,
  `home` varchar(255) default NULL,
  `description` text,
  `owner` varchar(100) default NULL,
  `pilot` varchar(100) default NULL,
  `copilot` varchar(100) default NULL,
  `dest` varchar(255) default NULL,
  `type` int(5) default NULL,
  `class` int(5) default NULL,
  `comm` int(10) default NULL,
  `sensor` int(10) default NULL,
  `astro_array` int(10) default NULL,
  `hyperspeed` int(10) default NULL,
  `hyperdistance` int(10) default NULL,
  `realspeed` int(10) default NULL,
  `currspeed` int(10) default NULL,
  `shipstate` int(10) default NULL,
  `statet0` int(5) default NULL,
  `statet1` int(5) default NULL,
  `statet2` int(5) default NULL,
  `missiletype` int(5) default NULL,
  `missilestate` int(5) default NULL,
  `missiles` int(10) default NULL,
  `maxmissiles` int(10) default NULL,
  `torpedos` int(10) default NULL,
  `maxtorpedos` int(10) default NULL,
  `rockets` int(10) default NULL,
  `maxrockets` int(10) default NULL,
  `lasers` int(10) default NULL,
  `tractorbeam` int(10) default NULL,
  `manuever` int(10) default NULL,
  `bayopen` int(5) default NULL,
  `hatchopen` int(5) default NULL,
  `autorecharge` int(5) default NULL,
  `autotrack` int(5) default NULL,
  `autospeed` int(10) default NULL,
  `vx` int(10) default NULL,
  `vy` int(10) default NULL,
  `vz` int(10) default NULL,
  `hx` int(10) default NULL,
  `hy` int(10) default NULL,
  `hz` int(10) default NULL,
  `jx` int(10) default NULL,
  `jy` int(10) default NULL,
  `jz` int(10) default NULL,
  `maxenergy` int(10) default NULL,
  `energy` int(10) default NULL,
  `shield` int(10) default NULL,
  `maxshield` int(10) default NULL,
  `hull` int(10) default NULL,
  `maxhull` int(10) default NULL,
  `cockpit` int(10) default NULL,
  `turret1` int(10) default NULL,
  `turret2` int(10) default NULL,
  `location` int(10) default NULL,
  `lastdoc` int(10) default NULL,
  `shipyard` int(10) default NULL,
  `entrance` int(10) default NULL,
  `hanger` int(10) default NULL,
  `engineroom` int(10) default NULL,
  `firstroom` int(10) default NULL,
  `lastroom` int(10) default NULL,
  `navseat` int(10) default NULL,
  `pilotseat` int(10) default NULL,
  `coseat` int(10) default NULL,
  `gunseat` int(10) default NULL,
  `collision` int(10) default NULL,
  `target0` int(10) default NULL,
  `target1` int(10) default NULL,
  `target2` int(10) default NULL,
  `currjump` int(10) default NULL,
  `chaff` int(10) default NULL,
  `maxchaff` int(10) default NULL,
  `chaff_released` int(10) default NULL,
  `autopilot` int(10) default NULL,
  `inroom` int(20) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `ships`
--


/*!40000 ALTER TABLE `ships` DISABLE KEYS */;
LOCK TABLES `ships` WRITE;
INSERT INTO `ships` VALUES (1,1,'My Ship','Coruscant','This ship totally rocks','Admin','Admin','Admin','0',1,1,10,10,10,100,0,100,0,0,0,0,0,0,0,0,0,2,2,5,5,50,56,24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10000,10000,1000,1000,1000,1000,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,10,0,0,100);
UNLOCK TABLES;
/*!40000 ALTER TABLE `ships` ENABLE KEYS */;

--
-- Table structure for table `starsystems`
--

DROP TABLE IF EXISTS `starsystems`;
CREATE TABLE `starsystems` (
  `id` int(3) default NULL,
  `name` varchar(100) default NULL,
  `x` int(10) default NULL,
  `y` int(10) default NULL,
  `flags` bigint(50) default NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `starsystems`
--


/*!40000 ALTER TABLE `starsystems` DISABLE KEYS */;
LOCK TABLES `starsystems` WRITE;
UNLOCK TABLES;
/*!40000 ALTER TABLE `starsystems` ENABLE KEYS */;

--
-- Table structure for table `votes`
--

DROP TABLE IF EXISTS `votes`;
CREATE TABLE `votes` (
  `id` int(3) NOT NULL auto_increment,
  `noteid` int(3) default NULL,
  `vote` int(3) default NULL,
  `player` varchar(100) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `votes`
--


/*!40000 ALTER TABLE `votes` DISABLE KEYS */;
LOCK TABLES `votes` WRITE;
INSERT INTO `votes` VALUES (1,3,1,'Admin');
UNLOCK TABLES;
/*!40000 ALTER TABLE `votes` ENABLE KEYS */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;


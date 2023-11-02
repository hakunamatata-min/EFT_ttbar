from HiggsAnalysis.CombinedLimit.PhysicsModel import *
import fnmatch
from math import sqrt,copysign 
sign = lambda x: copysign(1, x) 
#y0 = (-1.177641)
class stagex_ttwc_z(PhysicsModel):
	"Allow different signal strength fits for the stage-x model"
	def __init__(self):
		self.POIs = ""
		self.acttbar=False 
		self.acmu=False 
		self.accor=False 
		self.options= ""
		self.stage0= False 
		self.rvrf= False 
		self.singlemu= False 
		self.splitHad= False 
		self.muNames =[]
		self.pois = []
		self.count=0
	def setModelBuilder(self, modelBuilder):
		PhysicsModel.setModelBuilder(self, modelBuilder)
		self.modelBuilder.doModelBOnly = False
	def getYieldScale(self,bin,process):
		if "ttbar" not in process: # return 1 for background
			return 1
		else:
			print (process, "is a ttbar process with EW correction")
		if self.stage0:
			if "ttbar" in process:
				if self.acttbar:
					if "ci0010" in process:
						muname="r_tt_times_ci0010"
					elif "ci0000" in process:
						muname="r_tt_times_ci0000"
				else:
					muname="r_tt"
				print ("muname: %s" %muname)
	
		if self.acttbar:
			if not self.acmu:
				# x:Cpq3, y: Cpu, z:ReCup, k: ImCup
				self.modelBuilder.doVar("z[0.0,-5,5]" )			
				self.modelBuilder.factory_("expr::r_tt_times_ci0010(\"2*@0-@0*@0\", z)")
				self.modelBuilder.factory_("expr::r_tt_times_ci0000(\"(@0-1)*(@0-1)\", z)")
				self.pois.append("z")
			else:
				self.modelBuilder.doVar("z[0.0,-5,5]" )
				self.modelBuilder.doVar("mu[1.0,0,10]")	
				self.modelBuilder.factory_("expr::r_tt_times_ci0010(\"@1-(@0-1)*(@0-1)\", z, mu)")
				self.modelBuilder.factory_("expr::r_tt_times_ci0000(\"(@0-1)*(@0-1)\", z)")
				self.pois.append("z")
			self.POIs=",".join(self.pois)
			self.modelBuilder.doSet("POI",self.POIs)
			print ("parameters of interest in ttbar analysis: ", self.POIs)
			#kappa:sign(@1)*sqrt((1-abs(@0))*abs(@1)) @0:fcp @1:mu
			#kappa_t:sign(@0)*sign(@1)*sqrt(abs(@0)*abs(@1)) @0:fcp @1:mu
			
		if self.modelBuilder.out.var(muname):
			print ("reclying %s" %muname)
		else:                              
			if not "times" in muname:
				self.modelBuilder.doVar("%s[1,0,10]" % muname)
				print ("scale process %s with %s" %(process,muname))
				self.pois.append(muname)
				self.POIs=",".join(self.pois)
				self.modelBuilder.doSet("POI",self.POIs)
				print ("Default parameters of interest: ", self.POIs)
				
		return muname 
	def setPhysicsOptions(self,physOptions):
		for po in physOptions:
			if 'doStage0' in po: 
				self.stage0= True
				print ("doing stage0")
			if 'doacttbar' in po: 
				self.acttbar= True
				print ("doing tth AC ttbar")
			if 'domu' in po: 
				self.acmu= True
				print ("using mu parameter")	
			if 'singlemu' in po: 
				self.singlemu= True
				print ("doing single mu")

	def doParametersOfInterest(self):
		self.POIs=",".join(self.pois)
		print ("the parameters of interest: ", self.POIs)
		self.modelBuilder.doSet("POI",self.POIs)

stagex_ttwc_z = stagex_ttwc_z()

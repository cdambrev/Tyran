#pragma once


#include <fstream>
#include <iostream>
#include <string>
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/Misc/DateTime.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include <chrono>
#include <map>


//#define DEBUG_ON


struct Debugger {

private:
	Debugger() = default;

	FString pathText;
	FString pathAi;
	std::string logsAI;
	std::ofstream textLog;
	FString textStringLogger;
	FString aiStringLogger;


	struct VisualLog
	{
		VisualLog() = default;
		VisualLog(FString cat, FString pid, FString tid, long long ts, FString name) : cat{ cat }, pid{ pid }, tid{ tid }, ts{ ts }, name{ name } {}
		FString cat;
		FString pid;
		FString tid;
		long long ts;
		FString name;

		TArray<std::pair<FString, FString>> args;

	};

	std::map<std::pair<FString,FString>, VisualLog> visualLogs;

public:
	Debugger &operator=(const Debugger&) = delete;
	Debugger(const Debugger&) = delete;
	static Debugger &get() {
		static Debugger singleton;
		return singleton;
	}
	~Debugger() = default;


	void startNodeLog(UBehaviorTreeComponent& OwnerComp) {
		startVisualLog("IA Behaviour Tree", OwnerComp.GetOwner()->GetName(), "Comportement", OwnerComp.GetActiveNode()->GetName());
	}

	void endNodeLog(UBehaviorTreeComponent& OwnerComp) {
		endVisualLog(OwnerComp.GetOwner()->GetName(), "Comportement");
	}

	void addArgToNodeLog(UBehaviorTreeComponent& OwnerComp,FString name,FString argument) {
		addArgToVisualLog(OwnerComp.GetOwner()->GetName(), "Comportement", std::make_pair(name, argument));
	}


	void startVisualLog(FString cat, FString pid, FString tid,FString name) {
		using namespace std::chrono;
		visualLogs[std::make_pair(pid,tid)] = VisualLog(cat, pid, tid, duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count(), name);

	}

	void endVisualLog(FString pid, FString tid) {
		using namespace std::chrono;
		auto id = std::make_pair(pid, tid);
		aiStringLogger += "{";
		aiStringLogger += "\"cat\" : \"" + visualLogs[id].cat + "\",";
		aiStringLogger += "\"pid\" : \"" + pid + "\",";
		aiStringLogger += "\"tid\" : \"" + tid + "\",";
		aiStringLogger += "\"ts\": " + FString::FromInt(visualLogs[id].ts) + ", ";
		aiStringLogger += "\"ph\" : \"B\",";
		aiStringLogger += "\"name\" : \"" + visualLogs[id].name + "\",";
		aiStringLogger += "\"args\" : {";
		for (auto a : visualLogs[id].args) {
			aiStringLogger += "\"" + a.first + "\" : \"" + a.second + "\",";
		}

		if(visualLogs[id].args.Num() > 0)
			aiStringLogger = aiStringLogger.LeftChop(1);

		aiStringLogger += "}";
		aiStringLogger += "},";


		aiStringLogger += "{";
		aiStringLogger += "\"cat\" : \"" + visualLogs[id].cat + "\",";
		aiStringLogger += "\"pid\" : \"" + pid + "\",";
		aiStringLogger += "\"tid\" : \"" + tid + "\",";
		aiStringLogger += "\"ts\": " + FString::FromInt(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()) + ", ";
		aiStringLogger += "\"ph\" : \"E\",";
		aiStringLogger += "\"name\" : \"" + visualLogs[id].name + "\"";
		aiStringLogger += "},";

		FFileHelper::SaveStringToFile(aiStringLogger + "{} ]", *pathAi);
	}



	void addArgToVisualLog(FString pid, FString tid, std::pair<FString, FString> p) {
		visualLogs[std::make_pair(pid, tid)].args.Add(p);
	}

	void addTextLog(FString s, FString cat) {
		textStringLogger += "<tr class='" + cat;
		textStringLogger += "'><th class='heure'>[" + FString::FromInt(FDateTime::Now().GetHour()) + ":" + FString::FromInt(FDateTime::Now().GetMinute()) + ":" + FString::FromInt(FDateTime::Now().GetSecond()) + "]</th><th>" + s;
		textStringLogger += "</th></tr>";
		FFileHelper::SaveStringToFile(textStringLogger + "</table></body></html>", *pathText);

	}


	//Demarre et reset le fichier log
	void startTextLog() {
		FString SaveDirectory = FPaths::ProjectDir() + "Logs/";

		// Get absolute file path
		pathText = SaveDirectory + "textLog_" + FString::FromInt(FDateTime::Now().ToUnixTimestamp()) + ".html";
		textStringLogger = "<!DOCTYPE html><html><head><link rel='stylesheet' type='text/css' href='LogsPresentation/debug.css'><script src='LogsPresentation/debug.js'></script><title> Log " + FString::FromInt(FDateTime::Now().ToUnixTimestamp()) + "</title></head><div id = 'menu'></div><body><table>";
	}

	//Demarre et reset le fichier log
	void startAILog() {
		FString SaveDirectory = FPaths::ProjectDir() + "Logs/";

		// Get absolute file path
		pathAi = SaveDirectory + "AILog_" + FString::FromInt(FDateTime::Now().ToUnixTimestamp()) + ".json";
		aiStringLogger = "[";
	}
};









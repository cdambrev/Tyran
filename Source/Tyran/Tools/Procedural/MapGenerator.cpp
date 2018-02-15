// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGenerator.h"
#include <EngineUtils.h>
#include <ConstructorHelpers.h>

// Sets default values
AMapGenerator::AMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	nbIterations = 9;
	blockSize = 6000;
	static ConstructorHelpers::FClassFinder<AActor> helpB(TEXT("/Game/Objects/GenerationBlock/GB_Bunker"));
	static ConstructorHelpers::FClassFinder<AActor> helpBRH(TEXT("/Game/Objects/GenerationBlock/GB_BigRoadHorizontal"));
	static ConstructorHelpers::FClassFinder<AActor> helpBRV(TEXT("/Game/Objects/GenerationBlock/GB_BigRoadVertical"));
	static ConstructorHelpers::FClassFinder<AActor> helpBRXC(TEXT("/Game/Objects/GenerationBlock/GB_BigRoadXCrossroad"));
	static ConstructorHelpers::FClassFinder<AActor> helpBRHEL(TEXT("/Game/Objects/GenerationBlock/GB_BigRoadHorizontalEndLeft"));
	static ConstructorHelpers::FClassFinder<AActor> helpBRHER(TEXT("/Game/Objects/GenerationBlock/GB_BigRoadHorizontalEndRight"));
	static ConstructorHelpers::FClassFinder<AActor> helpBRVEN(TEXT("/Game/Objects/GenerationBlock/GB_BigRoadVerticalEndNorth"));
	static ConstructorHelpers::FClassFinder<AActor> helpBRVES(TEXT("/Game/Objects/GenerationBlock/GB_BigRoadVerticalEndSouth"));

	GenerationBrick * B = new GenerationBrick();
	B->objectToBuild = helpB.Class;
	Defaultbricks.Add(B);
	
	GenerationBrick * BRH = new GenerationBrick();
	BRH->objectToBuild = helpBRH.Class;
	Defaultbricks.Add(BRH);

	GenerationBrick * BRV = new GenerationBrick();
	BRV->objectToBuild = helpBRV.Class;
	Defaultbricks.Add(BRV);

	GenerationBrick * BRXC = new GenerationBrick();
	BRXC->objectToBuild = helpBRXC.Class;
	Defaultbricks.Add(BRXC);

	GenerationBrick * BRVEN = new GenerationBrick();
	BRVEN->objectToBuild = helpBRVEN.Class;
	Defaultbricks.Add(BRVEN);

	GenerationBrick * BRVES = new GenerationBrick();
	BRVES->objectToBuild = helpBRVES.Class;
	Defaultbricks.Add(BRVES);

	GenerationBrick * BRHER = new GenerationBrick();
	BRHER->objectToBuild = helpBRHER.Class;
	Defaultbricks.Add(BRHER);

	GenerationBrick * BRHEL = new GenerationBrick();
	BRHEL->objectToBuild = helpBRHEL.Class;
	Defaultbricks.Add(BRHEL);

	GenerationBrick * BRVSB = new GenerationBrick();
	Defaultbricks.Add(BRVSB);

	GenerationBrick * BRVST = new GenerationBrick();
	Defaultbricks.Add(BRVST);

	GenerationBrick * BRHSL = new GenerationBrick();
	Defaultbricks.Add(BRHSL);

	GenerationBrick * BRHSR = new GenerationBrick();
	Defaultbricks.Add(BRHSR);

	//Seed
	GenerationBrick::Transformation baseBunker;
	baseBunker.center = B;
	baseBunker.north = BRVST;
	baseBunker.east = BRHSR;
	baseBunker.south = BRVSB;
	baseBunker.west = BRHSL;

	startPoint.addTransformation(baseBunker, 1);

	//BRHSR
	GenerationBrick::Transformation extendRight;
	extendRight.center = BRH;
	extendRight.east = BRHSR;
	
	GenerationBrick::Transformation crossXRight;
	crossXRight.center = BRXC;
	crossXRight.north = BRVST;
	crossXRight.south = BRVSB;
	crossXRight.east = BRHSR;

	GenerationBrick::Transformation endRight;
	endRight.center = BRHER;

	BRHSR->addTransformation(extendRight, 25);
	BRHSR->addTransformation(crossXRight, 5);
	BRHSR->addTransformation(endRight, 1);
	BRHSR->finalTransformation = endRight;

	//BRHSL
	GenerationBrick::Transformation extendLeft;
	extendLeft.center = BRH;
	extendLeft.west = BRHSL;

	GenerationBrick::Transformation crossXLeft;
	crossXLeft.center = BRXC;
	crossXLeft.north = BRVST;
	crossXLeft.south = BRVSB;
	crossXLeft.west = BRHSL;

	GenerationBrick::Transformation endLeft;
	endLeft.center = BRHEL;

	BRHSL->addTransformation(extendLeft, 25);
	BRHSL->addTransformation(crossXLeft, 5);
	BRHSL->addTransformation(endLeft, 1);
	BRHSL->finalTransformation = endLeft;

	//BRVST
	GenerationBrick::Transformation extendTop;
	extendTop.center = BRV;
	extendTop.north = BRVST;

	GenerationBrick::Transformation crossXTop;
	crossXTop.center = BRXC;
	crossXTop.north = BRVST;
	crossXTop.east = BRHSR;
	crossXTop.west = BRHSL;

	GenerationBrick::Transformation endTop;
	endTop.center = BRVEN;

	BRVST->addTransformation(extendTop, 25);
	BRVST->addTransformation(crossXTop, 5);
	BRVST->addTransformation(endTop, 1);
	BRVST->finalTransformation = endTop;

	//BRVSB
	GenerationBrick::Transformation extendBot;
	extendBot.center = BRV;
	extendBot.south = BRVSB;

	GenerationBrick::Transformation crossXBot;
	crossXBot.center = BRXC;
	crossXBot.south = BRVSB;
	crossXBot.east = BRHSR;
	crossXBot.west = BRHSL;

	GenerationBrick::Transformation endBot;
	endBot.center = BRVES;

	BRVSB->addTransformation(extendBot, 25);
	BRVSB->addTransformation(crossXBot, 5);
	BRVSB->addTransformation(endBot, 1);
	BRVSB->finalTransformation = endBot;

	//MERGE TEMPLATES
	mergeTemplates.Add(MergeTemplate{helpBRH.Class, helpBRV.Class, helpBRXC.Class});
	mergeTemplates.Add(MergeTemplate{helpBRH.Class, helpBRXC.Class, helpBRXC.Class });
	mergeTemplates.Add(MergeTemplate{helpBRV.Class, helpBRXC.Class, helpBRXC.Class });
}

AMapGenerator::~AMapGenerator()
{
	for (auto b : Defaultbricks) {
		delete b;
	}
}

// Called when the game starts or when spawned
void AMapGenerator::BeginPlay()
{
	if (Role == ROLE_Authority) {
		startPoint.posX = 0;
		startPoint.posY = 0;
		bricks.Add(startPoint);
		for (int i = 0; i < nbIterations; ++i) {
			for (auto b : bricks) {
				if (b.isTransformable()) {
					auto trans = b.getTransformation();
					if (trans.center) {
						GenerationBrick gb = *trans.center;
						gb.posX = b.posX;
						gb.posY = b.posY;
						tempBricks.Add(gb);
					}
					if (trans.north) {
						GenerationBrick gb = *trans.north;
						gb.posX = b.posX;
						gb.posY = b.posY + blockSize;
						tempBricks.Add(gb);
					}
					if (trans.south) {
						GenerationBrick gb = *trans.south;
						gb.posX = b.posX;
						gb.posY = b.posY - blockSize;
						tempBricks.Add(gb);
					}
					if (trans.west) {
						GenerationBrick gb = *trans.west;
						gb.posX = b.posX + blockSize;
						gb.posY = b.posY;
						tempBricks.Add(gb);
					}
					if (trans.east) {
						GenerationBrick gb = *trans.east;
						gb.posX = b.posX - blockSize;
						gb.posY = b.posY;
						tempBricks.Add(gb);
					}
					if (trans.southEast) {
						GenerationBrick gb = *trans.southEast;
						gb.posX = b.posX - blockSize;
						gb.posY = b.posY - blockSize;
						tempBricks.Add(gb);
					}
					if (trans.southWest) {
						GenerationBrick gb = *trans.southWest;
						gb.posX = b.posX + blockSize;
						gb.posY = b.posY - blockSize;
						tempBricks.Add(gb);
					}
					if (trans.northEast) {
						GenerationBrick gb = *trans.northEast;
						gb.posX = b.posX - blockSize;
						gb.posY = b.posY + blockSize;
						tempBricks.Add(gb);
					}
					if (trans.northWest) {
						GenerationBrick gb = *trans.northWest;
						gb.posX = b.posX + blockSize;
						gb.posY = b.posY + blockSize;
						tempBricks.Add(gb);
					}
				}
				else {
					tempBricks.Add(b);
				}
			}
			bricks.Empty();
			bricks = tempBricks;
			tempBricks.Empty();
		}
		//Final transformation
		for (auto b : bricks) {
			if (b.isTransformable()) {
				auto trans = b.finalTransformation;
				if (trans.center) {
					GenerationBrick gb = *trans.center;
					gb.posX = b.posX;
					gb.posY = b.posY;
					tempBricks.Add(gb);
				}
				if (trans.north) {
					GenerationBrick gb = *trans.north;
					gb.posX = b.posX;
					gb.posY = b.posY + blockSize;
					tempBricks.Add(gb);
				}
				if (trans.south) {
					GenerationBrick gb = *trans.south;
					gb.posX = b.posX;
					gb.posY = b.posY - blockSize;
					tempBricks.Add(gb);
				}
				if (trans.west) {
					GenerationBrick gb = *trans.west;
					gb.posX = b.posX + blockSize;
					gb.posY = b.posY;
					tempBricks.Add(gb);
				}
				if (trans.east) {
					GenerationBrick gb = *trans.east;
					gb.posX = b.posX - blockSize;
					gb.posY = b.posY;
					tempBricks.Add(gb);
				}
				if (trans.southEast) {
					GenerationBrick gb = *trans.southEast;
					gb.posX = b.posX - blockSize;
					gb.posY = b.posY - blockSize;
					tempBricks.Add(gb);
				}
				if (trans.southWest) {
					GenerationBrick gb = *trans.southWest;
					gb.posX = b.posX + blockSize;
					gb.posY = b.posY - blockSize;
					tempBricks.Add(gb);
				}
				if (trans.northEast) {
					GenerationBrick gb = *trans.northEast;
					gb.posX = b.posX - blockSize;
					gb.posY = b.posY + blockSize;
					tempBricks.Add(gb);
				}
				if (trans.northWest) {
					GenerationBrick gb = *trans.northWest;
					gb.posX = b.posX + blockSize;
					gb.posY = b.posY + blockSize;
					tempBricks.Add(gb);
				}
			}
			else {
				tempBricks.Add(b);
			}
		}
		bricks.Empty();
		bricks = tempBricks;
		tempBricks.Empty();
		//Merge of overlapping parts
		for (int i = 0; i < bricks.Num(); ++i) {
			if (!bricks[i].invalidatedAtMerge) {
				int concurrent = -1;
				for (int j = i + 1; j < bricks.Num(); ++j) {
					if (bricks[i].posX == bricks[j].posX && bricks[i].posY == bricks[j].posY) {
						concurrent = j;
					}
				}
				if (concurrent == -1) {
					tempBricks.Add(bricks[i]);
				}
				else {
					MergeTemplate * found = nullptr;
					for (auto m : mergeTemplates) {
						if ((m.source1 == bricks[i].objectToBuild && m.source2 == bricks[concurrent].objectToBuild) || (m.source2 == bricks[i].objectToBuild && m.source1 == bricks[concurrent].objectToBuild)) {
							found = &m;
						}
					}
					if (found) {
						GenerationBrick mergedBrick;
						mergedBrick.objectToBuild = found->resultBuild;
						tempBricks.Add(mergedBrick);
						bricks[concurrent].invalidatedAtMerge = true;
					}
					else {
						tempBricks.Add(bricks[i]);
					}
				}
			}
		}
		bricks.Empty();
		bricks = tempBricks;
		tempBricks.Empty();
		//Finalization and build
		for (auto b : bricks) {
			FVector loc = GetActorTransform().GetLocation();
			loc.X += b.posX;
			loc.Y += b.posY;
			GetWorld()->SpawnActor<AActor>(b.objectToBuild, FTransform(GetActorTransform().GetRotation(), loc));
		}
	}
	Destroy();
}


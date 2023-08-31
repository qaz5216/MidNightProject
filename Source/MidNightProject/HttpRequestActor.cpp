// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpRequestActor.h"
#include "HttpModule.h"
#include "JsonParseLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "instrument.h"
#include "MidNightProjectCharacter.h"
#include "ChatWidget.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
// Sets default values
AHttpRequestActor::AHttpRequestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHttpRequestActor::BeginPlay()
{
	Super::BeginPlay();
	
	instrument = Cast<Ainstrument>(UGameplayStatics::GetActorOfClass(this, Ainstrument::StaticClass()));
	player = Cast<AMidNightProjectCharacter>(UGameplayStatics::GetActorOfClass(this, AMidNightProjectCharacter::StaticClass()));
}

// Called every frame
void AHttpRequestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHttpRequestActor::PostRequset(const FString url)
{
	TMap<FString, FString> chatData;
	//studentData.Add("Name", "AAA");
	//studentData.Add("Age", "30");
	//studentData.Add("Height", "172");
	
	//GEngine->AddOnScreenDebugMessage(-1, 3.0, FColor::Yellow, FString::Printf(TEXT("PostRequset URL")));
	GEngine->AddOnScreenDebugMessage(-1, 3.0, FColor::Yellow, url);

	chatData.Add("id", FText::AsNumber(instrument->id).ToString());
	chatData.Add("content",  player->Chat_UI->Etext_Q->GetText().ToString());

	FString chatJsonData = UJsonParseLibrary::MakeJson(chatData);


	//����� �����ϰ�, request �ν��Ͻ��� �����Ѵ�.
	FHttpModule& httpModule = FHttpModule::Get();
	//��û ����
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();
	req->SetURL(url);
	req->SetVerb(TEXT("POST"));
	//req->SetHeader(TEXT("Content-Type"), TEXT("audio/x-wav"));
	req->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// wav �迭 ��ȯ
	/*TArray<uint8> myVoiceRaw;
	FString filePath= FPaths::ProjectContentDir() + "/EunTest";
	FFileHelper::LoadFileToArray(myVoiceRaw, *filePath);
	req->SetContent(myVoiceRaw);*/
	req->SetContentAsString(chatJsonData);
	req->OnProcessRequestComplete().BindUObject(this, &AHttpRequestActor::OnPostData);
	//req->OnProcessRequestComplete().BindUObject(this, &AHttpRequestActor::OnPostData);
	req->ProcessRequest();
}

void AHttpRequestActor::OnPostData(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, FString::Printf(TEXT("OnPostData Success")));

		FString receivedData = Response->GetContentAsString();

		//FJsonSerializer::Deserialize();
		//���� �����͸� ȭ�鿡 ����Ѵ�.
		//gm->SetLogText(receivedData);

		FString parsedData = UJsonParseLibrary::JsonParse(receivedData);
		player->Chat_UI->text_Answer->SetText(FText::FromString(parsedData));
		player->Chat_UI->text_waiting->SetVisibility(ESlateVisibility::Hidden);
		player->Chat_UI->text_Answer->SetVisibility(ESlateVisibility::Visible);
	}

	else {

		GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, FString::Printf(TEXT("OnPostData Fail")));

		// ��û ���� ���� Ȯ��
		EHttpRequestStatus::Type status = Request->GetStatus();

		switch (status)
		{
		case EHttpRequestStatus::NotStarted:
			break;
		case EHttpRequestStatus::Processing:
			break;
		case EHttpRequestStatus::Failed:
			break;
		case EHttpRequestStatus::Failed_ConnectionError:
			break;
		case EHttpRequestStatus::Succeeded:
			break;
		default:
			break;
		}


	}
	//���� �ڵ� Ȯ��
	int32 responseCode = Response->GetResponseCode();
	//player->Chat_UI->text_Answer->SetText(FText::AsNumber(responseCode));
	//gm->SetLogText(FString::Printf(TEXT("Response Code : %d"), responseCode));
}

void AHttpRequestActor::OnPostVoiceData(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, FString::Printf(TEXT("OnPostVoiceData Success")));

		//FString receivedData = Response->GetContentAsString();
		TArray<uint8> receivedData = Response->GetContent();

		//���� PCM���Ϸ� ��ȯ�ϸ鼭 ������ 
	
		UJsonParseLibrary::PlayPCMData(receivedData, player);
		//player->Chat_UI->text_Answer->SetText(FText::FromString(parsedData));
		//player->Chat_UI->text_waiting->SetVisibility(ESlateVisibility::Hidden);
		//player->Chat_UI->text_Answer->SetVisibility(ESlateVisibility::Visible);
	}

	else {

		GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, FString::Printf(TEXT("OnPostVoiceData Fail")));

		// ��û ���� ���� Ȯ��
		EHttpRequestStatus::Type status = Request->GetStatus();

		switch (status)
		{
		case EHttpRequestStatus::NotStarted:
			break;
		case EHttpRequestStatus::Processing:
			break;
		case EHttpRequestStatus::Failed:
			break;
		case EHttpRequestStatus::Failed_ConnectionError:
			break;
		case EHttpRequestStatus::Succeeded:
			break;
		default:
			break;
		}


	}
	//���� �ڵ� Ȯ��
	int32 responseCode = Response->GetResponseCode();
	//player->Chat_UI->text_Answer->SetText(FText::AsNumber(responseCode));
	//gm->SetLogText(FString::Printf(TEXT("Response Code : %d"), responseCode));
}




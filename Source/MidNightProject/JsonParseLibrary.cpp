// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonParseLibrary.h"
#include "Serialization/JsonSerializer.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWaveProcedural.h"
#include "Kismet/GameplayStatics.h"
#include "MidNightProjectCharacter.h"
#include "RuntimeAudioImporterLibrary.h"
#include "Serialization/EditorBulkData.h"


FString UJsonParseLibrary::JsonParse(const FString& originData)
{
	FString parsedData;

	//josn데이터 파싱 방법
	//1.Reader or  Writer(번역기) 를 만든다.
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(originData);

	//2.Reader 로 파싱(구문분석)된 결괄르 json Object 를 선언한다.
	TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());

	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Yellow, FString::Printf(TEXT("JsonParse")));

	//3. 해독하기 (Deserialize: 비직렬화 , Serialize : 직렬화)
	if (FJsonSerializer::Deserialize(reader, result))
	{
		//result->GetObjectField("content");
	   //result->GetObjectFiled("content");
	   //result->TryGetObjectField("contnet",);

	   //json value중 items라는 value값을 찾아옴 value 중 정영화 되지 않은 최상단 부모객체 : FJsonValue
		TArray<TSharedPtr<FJsonValue>> parsedDataArray = result->GetArrayField("items");

		//parsedDataArray->As

		GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Yellow, FString::Printf(TEXT("Deserialize")));

		//item 배열 원소 하나하나당  찾으면서 어떤 객체를 찾을건지 정하기
		for (TSharedPtr<FJsonValue> pd : parsedDataArray)
		{
			parsedData = pd->AsObject()->GetStringField("content");
			//FString authorName = pd->AsObject()->GetStringField("aut_nm");
			//parsedData.Append(FString::Printf(TEXT("BookName : %s\n AuthorName : %s\n\n"), *bookdName, *authorName));
			//GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Yellow, text_answer);
		}

	}

	return parsedData;
}

FString UJsonParseLibrary::MakeJson(const TMap<FString, FString> source)
{
	// josn Object를 생성하고 값을 넣는다. {} 만 있는 상태
	TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject());

	for (TPair<FString, FString> kv : source)
	{
		// 키와 value 값 넣기
		jsonObj->SetStringField(kv.Key, kv.Value);

	}

	// Writer를 생성하고, json object 인코딩한다.
	FString jsonData;

	TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<TCHAR>::Create(&jsonData);
	FJsonSerializer::Serialize(jsonObj.ToSharedRef(), writer);

	return jsonData;
}



void UJsonParseLibrary::PlayPCMData(const TArray<uint8>& PCMData, class AMidNightProjectCharacter* player)
{
	//Audio 컴포넌트 생성
	//AMidNightProjectCharacter* player = Cast<AMidNightProjectCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AMidNightProjectCharacter::StaticClass()));


	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, FString::Printf(TEXT("PlayPCMData Success")));
	//UAudioComponent* AudioComponent = UGameplayStatics::CreateSound2D(this, nullptr);
	// PCM 데이터를 Wave 포맷으로 변환
	USoundWaveProcedural* SoundWave = NewObject<USoundWaveProcedural>();
	SoundWave->SetSampleRate(44100); // 적절한 샘플 레이트 설정
	SoundWave->NumChannels = 1; // 모노 오디오
	SoundWave->Duration = INDEFINITELY_LOOPING_DURATION; // 무한 루프 설정
	SoundWave->SoundGroup = SOUNDGROUP_Default;

	TArray<uint8> ByteData;
	ByteData.AddUninitialized(PCMData.Num() * sizeof(uint16));
	FMemory::Memcpy(ByteData.GetData(), PCMData.GetData(), ByteData.Num());
	SoundWave->QueueAudio(ByteData.GetData(), ByteData.Num());


	// 받은 데이터를 wav 파일로 저장4141
	FString saveURL = FPaths::ProjectPersistentDownloadDir() + FString("/MyWave.wav");
	bool isSaved = FFileHelper::SaveArrayToFile(ByteData, *saveURL);
	FString saveURL2 = FPaths::ProjectPersistentDownloadDir() + FString("/MyPCM.wav");
	bool isSaved2 = FFileHelper::SaveArrayToFile(PCMData, *saveURL2);
	UE_LOG(LogTemp, Warning, TEXT("Save Result: %s"), isSaved ? *FString("Save Successfully") : *FString("Faild Saving File"));
	UE_LOG(LogTemp, Warning, TEXT("Save Result: %s"), isSaved2 ? *FString("Save Successfully") : *FString("Faild Saving File"));


	// Audio 컴포넌트에 Wave 설정 후 재생
	/*AudioComponent->SetSound(SoundWave);
	AudioComponent->Play();*/
	//player->audiocomp->SetSound(SoundWave);
	if (player->audiocomp)
	{
		//RuntimeAudioImporter = URuntimeAudioImporterLibrary::CreateRuntimeAudioImporter();
		//RuntimeAudioImporter->OnProgressNative
		USoundWave* myWavSound = LoadObject<USoundWave>(nullptr, *saveURL);

		player->audiocomp->SetSound(myWavSound);
		player->audiocomp->Play();
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, FString::Printf(TEXT("Sound Fail")));

	}

}






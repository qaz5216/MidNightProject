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

	//josn������ �Ľ� ���
	//1.Reader or  Writer(������) �� �����.
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(originData);

	//2.Reader �� �Ľ�(�����м�)�� ����� json Object �� �����Ѵ�.
	TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());

	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Yellow, FString::Printf(TEXT("JsonParse")));

	//3. �ص��ϱ� (Deserialize: ������ȭ , Serialize : ����ȭ)
	if (FJsonSerializer::Deserialize(reader, result))
	{
		//result->GetObjectField("content");
	   //result->GetObjectFiled("content");
	   //result->TryGetObjectField("contnet",);

	   //json value�� items��� value���� ã�ƿ� value �� ����ȭ ���� ���� �ֻ�� �θ�ü : FJsonValue
		TArray<TSharedPtr<FJsonValue>> parsedDataArray = result->GetArrayField("items");

		//parsedDataArray->As

		GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Yellow, FString::Printf(TEXT("Deserialize")));

		//item �迭 ���� �ϳ��ϳ���  ã���鼭 � ��ü�� ã������ ���ϱ�
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
	// josn Object�� �����ϰ� ���� �ִ´�. {} �� �ִ� ����
	TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject());

	for (TPair<FString, FString> kv : source)
	{
		// Ű�� value �� �ֱ�
		jsonObj->SetStringField(kv.Key, kv.Value);

	}

	// Writer�� �����ϰ�, json object ���ڵ��Ѵ�.
	FString jsonData;

	TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<TCHAR>::Create(&jsonData);
	FJsonSerializer::Serialize(jsonObj.ToSharedRef(), writer);

	return jsonData;
}



void UJsonParseLibrary::PlayPCMData(const TArray<uint8>& PCMData, class AMidNightProjectCharacter* player)
{
	//Audio ������Ʈ ����
	//AMidNightProjectCharacter* player = Cast<AMidNightProjectCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AMidNightProjectCharacter::StaticClass()));


	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, FString::Printf(TEXT("PlayPCMData Success")));
	//UAudioComponent* AudioComponent = UGameplayStatics::CreateSound2D(this, nullptr);
	// PCM �����͸� Wave �������� ��ȯ
	USoundWaveProcedural* SoundWave = NewObject<USoundWaveProcedural>();
	SoundWave->SetSampleRate(44100); // ������ ���� ����Ʈ ����
	SoundWave->NumChannels = 1; // ��� �����
	SoundWave->Duration = INDEFINITELY_LOOPING_DURATION; // ���� ���� ����
	SoundWave->SoundGroup = SOUNDGROUP_Default;

	TArray<uint8> ByteData;
	ByteData.AddUninitialized(PCMData.Num() * sizeof(uint16));
	FMemory::Memcpy(ByteData.GetData(), PCMData.GetData(), ByteData.Num());
	SoundWave->QueueAudio(ByteData.GetData(), ByteData.Num());


	// ���� �����͸� wav ���Ϸ� ����4141
	FString saveURL = FPaths::ProjectPersistentDownloadDir() + FString("/MyWave.wav");
	bool isSaved = FFileHelper::SaveArrayToFile(ByteData, *saveURL);
	FString saveURL2 = FPaths::ProjectPersistentDownloadDir() + FString("/MyPCM.wav");
	bool isSaved2 = FFileHelper::SaveArrayToFile(PCMData, *saveURL2);
	UE_LOG(LogTemp, Warning, TEXT("Save Result: %s"), isSaved ? *FString("Save Successfully") : *FString("Faild Saving File"));
	UE_LOG(LogTemp, Warning, TEXT("Save Result: %s"), isSaved2 ? *FString("Save Successfully") : *FString("Faild Saving File"));


	// Audio ������Ʈ�� Wave ���� �� ���
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






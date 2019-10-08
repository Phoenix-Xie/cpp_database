#include "database.h"
#include "statu.h"
#include <string>
using namespace std;

//Ī������ľ�̬˽�б�����ʼ�� 
DataBase * DataBase::DBinstance = NULL;

DataBase::DataBase(){
	//ȫ������ 
	this->table_name_max_len = 50;
	this->col_name_max_len = 50;
	this->table_number = 0; 
	FILE * fp = fopen(settings::settings_name.data(), "r");
	//��ȡ���б��� 
	if(fread(&this->table_number, sizeof(ll), 1, fp) == 0){
		this->table_number = 0;
	}
	//���ñ�������
	//���� 
	fclose(fp);
	//������ 
	fp = fopen(settings::table_settings_name.data(), "a");
	fclose(fp);
}

DataBase * DataBase::getInstance(){
	if(DBinstance == NULL){
		DBinstance = new DataBase(); 
	}
	return DBinstance;
}

void DataBase::closeDataBase(){
	FILE * fp = fopen("settings", "w");
	fwrite(&this->table_number, sizeof(ll), 1, fp);
	fclose(fp);
}

void DataBase::createTable(string name, vector<string> col_name, vector<ll> col_size, vector <char> isHash){
	
	//�������ļ� 
	vector<char> s ;
	FILE * fp = fopen(settings::table_settings_name.data(), "rb+");
	fseek(fp, 0, SEEK_END);
	char valid = 'T';
	//д����Ч��־λ
	// printf("%d\n", fwrite(&valid, sizeof(char), 1, fp)); 
	// fwrite(&valid, sizeof(char), 1, fp); 
	fwrite(&valid, sizeof(char), 1, fp); 
	//д���ļ��� 
	fwrite(name.data(), sizeof(char), this->table_name_max_len, fp);
	//д������ 
	ll len = col_size.size();
	fwrite(&len, sizeof(ll), 1, fp);
	//д��ÿһ������ 
	for(ll i = 0; i < len; i++){
		fwrite(col_name[i].data(), sizeof(char), this->col_name_max_len, fp);
	}

	//д��ÿһ�г��� 
	
	for(ll i = 0; i < len; i++){
		fwrite(&col_size[i], sizeof(ll), 1, fp);
	}
	fclose(fp);



	//���������ݿ��ļ�
	fp = fopen(name.data(), "w");
	
	rewind(fp);
	//д���ͷ
	ll nextLocation = -1;
	fwrite(&nextLocation, sizeof(ll), 1, fp);
	fclose(fp); 
	this->table_number++;
}

void DataBase::showTables(){
	printf("show\n");
	FILE * fp = fopen(settings::table_settings_name.data(), "rb+");
	fseek(fp, 0, SEEK_SET);
	// ѭ����ȡ
	char valid;
	while((valid=fgetc(fp))!=EOF){
		printf("tell:%d\n", ftell(fp));
		//��ȡ��Чλ
		printf("%c ", valid); 
		//��ȡ���� 
		vector <char> s(this->table_name_max_len);
		fread(&s[0], sizeof(char), this->table_name_max_len, fp);
		printf("%s ", &s[0]); 
		//��ȡ���� 
		ll len;
		fread(&len, sizeof(ll), 1, fp);
		printf("%d ", len);
		vector<char> col(this->col_name_max_len+10);
		for(ll j = 0; j < len; j++){
			fread(&col[0], sizeof(char), this->col_name_max_len, fp);
			printf("%s ", &col[0]);
		} 
		ll t;
		for(ll j = 0; j < len; j++){
			fread(&t, sizeof(ll), 1, fp);
			printf("%ld ", t);
		} 
		printf("\n"); 
	} 
	
	fclose(fp);
}

int DataBase::deleteTable(string name){
	FILE * fp = fopen(settings::settings_name.data(), "rb+");
	fseek(fp, 0, SEEK_SET);
	char valid; 
	vector <char> s(this->table_name_max_len);
	ll num; 
	while((valid = fgetc(fp)) != EOF){
		printf("tell:%d\n", ftell(fp));
		if(valid == 'T'){
			
			fread(&s[0], sizeof(char), this->table_name_max_len, fp);
			string ss(&s[0]);
			printf("��⵽ %s\n", &s[0]);
			if(ss == name){
				printf("����ɾ��\n"); 
				valid = 'F';
				fseek(fp, -((this->table_name_max_len+1) * sizeof(char)), SEEK_CUR);
				fwrite(&valid, sizeof(char), 1, fp);
				break;
			}
			fread(&num, sizeof(ll), 1, fp);
			fseek(fp, num * (this->col_name_max_len) * sizeof(char), SEEK_CUR);
			fseek(fp, num * sizeof(ll), SEEK_CUR);
		}
		else{
			fseek(fp, this->table_name_max_len * sizeof(char), SEEK_CUR);
			fread(&num, sizeof(ll), 1, fp);
			fseek(fp, num * (this->col_name_max_len) * sizeof(char), SEEK_CUR);
			fseek(fp, num * sizeof(ll), SEEK_CUR);
		}
	}
	//fflush(fp);
	fclose(fp);
}

string DataBase::getTableName(){
	return this->table_name;
}

ll DataBase::getColNum(){
	return this->table_col_num;
}

vector<string> DataBase::getColName(){
	return this->table_col_name;
}

string DataBase::chooseTable(string name){
	bool getFlag = false; //�Ƿ��ȡ����־ 
	FILE * fp = fopen(settings::table_settings_name.data(), "rb");
	fseek(fp, 0, SEEK_SET);
	//��ȡȫ�ֱ���
	ll clen = this->col_name_max_len;
	//�趨��ʱ���� 
	char valid; 
	vector <char> s(this->table_name_max_len);
	ll num; 
	ll lineNum = 0; //��ǰ�������к�
	while((valid = fgetc(fp)) != EOF){
		// printf("choose:%d\n", ftell(fp));
		//������Ч��Ӧ�ü����к�
		lineNum++;
		// �жϸ����Ƿ���Ч 
		if(valid == 'T'){
			fread(&s[0], sizeof(char), this->table_name_max_len,fp);
			string ss(&s[0]);
			//�ж��Ƿ�����ͬ���� 
			if(ss == name){
				printf("�ҵ�������ݱ�\n");
				this->table_id = lineNum;
				this->table_name = name;
				//��ȡ���� 
				fread(&num, sizeof(ll), 1, fp);
				this->table_col_num = num; 
				//��������
				vector<string> & cname = this->table_col_name;
				vector<ll> & csize = this->table_col_size;
				vector<ll> & csizep = this->table_col_pre_size;
				//��ʱ����
				vector<char> temp_c(clen); 
				//��ʼ������ 
				cname.resize(this->table_col_num);
				csize.resize(this->table_col_num);
				csizep.resize(this->table_col_num+1); 
				//��ȡÿһ������ 
				for(ll i = 0; i < num; i++){
					fread(&temp_c[0], sizeof(char), clen, fp);
					cname[i] = &temp_c[0];
				}
				//��ȡÿһ�еĿ�� 
				csizep[0] = 0;
				for(ll i = 0; i < num; i++){
					fread(&csize[i], sizeof(ll), 1, fp);
					//printf("csize:%d\n", csize[i]);
					csizep[i+1] = csizep[i] + csize[i];
				}
				getFlag = true; 
				break; 
			}
			//��ͬ������������� 
			else{
				ll num;
				fread(&num, sizeof(ll), 1, fp);
				//������������ �� �г��� 
				fseek(fp, num * sizeof(char) * clen, SEEK_CUR);
				fseek(fp, num * sizeof(ll), SEEK_CUR);
			} 
		}
		else{
			ll num;
			fseek(fp, sizeof(char) * this->table_name_max_len, SEEK_CUR);
			fread(&num, sizeof(ll), 1, fp);
			fseek(fp, num * sizeof(char) * clen, SEEK_CUR);
			fseek(fp, num * sizeof(ll), SEEK_CUR);
		} 
		//todo��������Ч���δ����
		//todo��δ�ҵ����δ���� 
	}
	fclose(fp); 
	if(getFlag){
		printf("%s", this->table_name.data());
		return name;
	}
	else{
		return "δ����";
	}
}

int DataBase::insert(const vector< vector<string> > & s, vector<ll> & id, string name){
	//�ж��Ƿ�ѡ��
	if (name == "" && (this->table_name == "δѡ��" || this->table_name == ""))
		return -2;
	//todo�����һ������ 
	printf("start insert\n");

	//��ʼ������
	id.clear();

	//�õ���������
	ll cnum = this->table_col_num; //����
	char valid = 'T';
	vector <ll> & csize = this->table_col_size;
	vector <ll> & cpsize = this->table_col_pre_size;
	ll totalSeek = this->table_col_pre_size[this->table_col_num] * sizeof(char);  //�����ܳ�
	ll lineSeek = sizeof(char) + sizeof(ll) + totalSeek; //ÿ�����ݳ���

	//���ļ�
	FILE * fp = fopen(this->table_name.data(), "rb+");
	rewind(fp);

	//��ȡ��ͷ
	ll nextLocation;
	fread(&nextLocation, sizeof(ll), 1, fp);
	ll n = s.size();
	for(int i = 0; i < n; i++){
		//ÿ�ζ�ȡ��ͷ
		if(nextLocation == -1){
			//�����ļ�ĩβ
			fseek(fp, 0, SEEK_END);
			//д���־λ
			fwrite(&valid, sizeof(char), 1, fp);
			
			//д����б�λ
			nextLocation = -1;
			fwrite(&nextLocation, sizeof(ll), 1, fp);

			ll m = this->table_col_num;
			
			for(ll j = 0;  j < m; j++){
				fwrite(s[i][j].data(), sizeof(char), csize[j], fp);
			}
		

		} else{ //�п�����
			//�ӿ�ʼ��ת������
			int temp;
			fseek(fp, nextLocation+sizeof(ll), SEEK_SET);
			// temp = ftell(fp);
			// printf("%d\n", temp);
			//д���־λ
			fwrite(&valid, sizeof(char), 1, fp);
			fflush(fp);
			// temp = ftell(fp);
			// printf("%d\n", temp);
			//��ȡ������nextֵ(�������)
			//ͬʱ��дҪ�ض�λ���������
			// fseek(fp, ftell(fp), SEEK_CUR);
			fread(&nextLocation, sizeof(ll), 1, fp);
			// temp = ftell(fp);
			// printf("%d\n", temp);
			// fseek(fp, ftell(fp)-sizeof(ll), SEEK_CUR);
			// temp = ftell(fp);
			// printf("%d\n", temp);
			ll tempNum = -1;
			fseek(fp, -sizeof(ll), SEEK_CUR);
			fwrite(&tempNum, sizeof(ll), 1, fp);	
			//д������
			ll m = this->table_col_num;
			for(ll j = 0;  j < m; j++){
				fwrite(s[i][j].data(), sizeof(char), csize[j], fp);
			}  
		}

		//�������������fpλ�ü���id
		ll fpLocation = ftell(fp);
		id.push_back((fpLocation-sizeof(ll))/lineSeek);
	}
	//�����ļ�ͷ
	rewind(fp);
	//�����һ�λ�õ�nextֵд��ͷ��
	fwrite(&nextLocation, sizeof(ll), 1, fp);
	fclose(fp);

	// ll cnum = this->table_col_num;
	// vector <ll> &csize = this->table_col_size;
	// FILE * fp = fopen(this->table_name.data() , "a");
	// // printf("name:%s\n", this->table_name.data());
	// ll line_num = s.size();
	// char valid = 'T';
	// for(ll i = 0; i < line_num; i++){
	// 	//д��������Чλ 
	// 	fwrite(&valid, sizeof(char), 1, fp);
	// 	for(ll j = 0; j < cnum; j++){
	// 		printf("%d %d %s %d\n", i, j, s[i][j].data(), csize[j]);
	// 		fwrite(s[i][j].data(), sizeof(char), csize[j], fp);
	// 	}
	// }
	// fclose(fp);
} 

void DataBase::showDatas(string name){
	if(name != "")
		this->chooseTable(name); 
	name = this->table_name; 

	//��������
	char valid;
	ll nextLocation;
	ll idx = 0;
	ll totalLen = sizeof(char) + sizeof(ll) + this->table_col_pre_size[this->table_col_num]*sizeof(char);

	FILE * fp = fopen(this->table_name.data(), "rb+");
	rewind(fp);
	fread(&nextLocation, sizeof(ll), 1, fp);
	
	printf("��ͷ��%lld  id:%lld\n", nextLocation, nextLocation/totalLen);
	

	//��ӡ��ͷ
	printf("��Чλ\t");
	printf("id\t");
	printf("nextLocation\t");
	for(ll i = 0; i < this->table_col_num; i++){
		printf("%s\t", this->table_col_name[i].data());
	} 
	printf("\n");
	//ȷ����ֶγ���
	ll maxLen = 0;
	for(ll i = 0; i < this->table_col_num; i++){
		maxLen = max(maxLen, this->table_col_size[i]);
	}
	vector <char> s(maxLen);
	string str; 
	//��ȡ����ʾÿһ�� 
	while((valid=fgetc(fp))!=EOF){
		idx++;
		//printf("tell:%d\n", ftell(fp));
		//��ȡ��Чλ
		printf("%c\t", valid); 
		printf("%lld\t", idx);
		//��ȡ����λ
		fread(&nextLocation, sizeof(ll), 1, fp);
		printf("%lld nextId:%lld\t", nextLocation, nextLocation/totalLen);
		//��ȡ����ÿһλ���ݲ���ӡ 
		for(ll i = 0; i < this->table_col_num; i++){
			fread(&s[0], sizeof(char), this->table_col_size[i], fp);
			
			printf("%s\t",  &s[0]);
		}  
		printf("\n");
	} 
	
	fclose(fp);
} 

int DataBase::deleteData(string key, string value, string name){
	//�ж��Ƿ�ѡ��
	if (name == "" && (this->table_name == "δѡ��" || this->table_name == ""))
		return -2;
	//���id
	vector<ll> id(0);
	vector<vector<string> > ans;
	this->query(key, value, id, ans);
	//��ʼ������
	ll n = id.size();
	ll len = sizeof(char) + sizeof(ll) + this->table_col_pre_size[this->table_col_num]*sizeof(char);
	char valid = 'F';
	ll nextLocation = 0;
	ll headLocation = 0;
	//���ļ�
	FILE * fp = fopen(this->table_name.data(), "rb+");
	fseek(fp, 0, SEEK_SET);
	fread(&headLocation, sizeof(ll), 1, fp);
	for(ll i = 0; i < n; i++){
		printf("tell%lld id:%lld\n", headLocation, id[i]);
		//�ӿ�ͷ��ת��ָ��λ��
		fseek(fp, len*(id[i]-1)+sizeof(ll), SEEK_SET);
		fwrite(&valid, sizeof(char), 1, fp);
		fwrite(&headLocation, sizeof(ll),1, fp);
		headLocation = (id[i]-1)*len;
	}
	rewind(fp);
	//������һ��ͨ���ļ�ͷ����
	fwrite(&headLocation, sizeof(ll), 1, fp);
	fclose(fp);
}

int DataBase::query(string key, string value, vector<ll> & id, vector< vector<string> > & ans, string name){
	//�ж��Ƿ�ѡ��
	if (name == "" && (this->table_name == "δѡ��" || this->table_name == ""))
		return -2;
	if(name != "")
		this->chooseTable(name);
	name = this->table_name; 

	//��ȡ��������
	ll idx, selfLen, preSeek, lastSeek, totalSeek;
	if(!this->getKeyLocation(key, idx, selfLen, preSeek, lastSeek, totalSeek)){
		//���޴��ֶ�
		return false;
	}
	//��ʼ������
	id.resize(0); //����id����
	ll lineNum = 0; //��¼��ǰ�кţ�����id
	ll maxLen = 0;
	for(ll i = 0; i < this->table_col_num; i++){
		maxLen = max(maxLen, this->table_col_size[i]);
	}
	vector<char> s(maxLen+10);  //��ʱ���� 
	string str;  //��ʱ���� 
	
	
	vector <ll> & csize = this->table_col_size;
	//���ļ���ѯ
	FILE * fp = fopen(name.data(), "rb");
	//����ͷ��
	fseek(fp, sizeof(ll), SEEK_SET);
	char valid;
	while((valid=fgetc(fp))!=EOF){
		lineNum++;
		//����������Ч 
		if(valid == 'T'){
			//��������λ
			fseek(fp, sizeof(ll), SEEK_CUR);
			//����ǰ�����ݿ�
			fseek(fp, preSeek, SEEK_CUR);
			//��ȡ���ݲ�����string
			fread(&s[0], sizeof(char), selfLen, fp);
			str = &s[0];
			// ������ȷ
			if(str == value){
				// printf("%s������ͬ", str.data());
				//�ҵ���ǰ��Ծ 
				fseek(fp, -(preSeek + selfLen*sizeof(char)), SEEK_CUR);
				//��ȡ��������
				vector<string> line(this->table_col_num); //һ�н�� 
				for(ll i = 0; i < this->table_col_num; i++){
					fread(&s[0], sizeof(char), csize[i], fp);
					line[i] = &s[0];
				}
				//vector�Ŀ�������� 
				ans.push_back(line); 
				id.push_back(lineNum);
			}
			else{
				//�������沿��
				fseek(fp, lastSeek, SEEK_CUR);
			}
		}
		else{
			fseek(fp, sizeof(ll), SEEK_CUR);
			fseek(fp, totalSeek, SEEK_CUR);
		}
	}
	return true;
}

int DataBase::queryById(vector<ll> &id, vector< vector<string> > & ans, ll id1, ll id2, string name){
	//�ж��Ƿ�ѡ��
	if (name == "" && (this->table_name == "δѡ��" || this->table_name == ""))
		return -2;
	//��������
	ans.clear();
	id.clear();
	if(id2 < 0){
		id2 = 0x3f3f3f3f3f3f3f3f;
	}
	if(id1 > id2)
		throw "id��Χ����id1ӦС��id2";
	//��������
	char valid; //��Чλ
	ll nextLocation; //��������λ
	ll idx = 0; //id��¼��
	ll totalSeek = this->table_col_pre_size[this->table_col_num] * sizeof(char);  //�����ܳ�
	ll lineSeek = sizeof(char) + sizeof(ll) + totalSeek; //ÿ�����ݳ���
	//ȷ����ֶγ���
	// todo: �ж��Ƿ���id������ǰ����,�Լ�id�Ϸ���
	ll maxLen = 0;
	for(ll i = 0; i < this->table_col_num; i++){
		maxLen = max(maxLen, this->table_col_size[i]);
	}
	vector <char> s(maxLen);
	string str; 
	
	FILE * fp = fopen(this->table_name.data(), "rb+");
	//������ͷ
	fseek(fp, sizeof(ll), SEEK_SET);
	//����ǰ�漸��
	fseek(fp, (id1-1)*lineSeek, SEEK_CUR);
	idx = id1;
	//��ȡ����ʾÿһ�� 
	while( ((valid=fgetc(fp))!=EOF) && idx <= id2){
		if(valid == 'T'){
			//������ͷ
			fseek(fp, sizeof(ll), SEEK_CUR);
			id.push_back(idx);
			vector<string> tempStr(this->table_col_num);
			for(ll i = 0; i < this->table_col_num; i++){
				fread(&s[0], sizeof(char), this->table_col_size[i], fp);
				tempStr[i] = &s[0];
			}  
			ans.push_back(tempStr);
		}
		else{
			fseek(fp, lineSeek-sizeof(char), SEEK_CUR);
		}
		idx++;
	}
	fclose(fp);
}

int DataBase::predictId(ll num, vector<ll> &id, string name){
	//�ж��Ƿ�ѡ��
	if (name == "" && (this->table_name == "δѡ��" || this->table_name == ""))
		return -2;
	if(name != "")
		this->chooseTable(name);
	name = this->table_name; 

	//��ȡ��������
	ll totalSeek = this->table_col_pre_size[this->table_col_num] * sizeof(char);  //�����ܳ�
	ll lineSeek = sizeof(char) + sizeof(ll) + totalSeek; //ÿ�����ݳ���
	//��ʼ������
	id.clear(); //����id����
	ll lineNum = 0; //��¼��ǰ�кţ�����id
	ll nextLocation; //��ͷ
	vector <ll> & csize = this->table_col_size;
	//���ļ���ѯ
	FILE * fp = fopen(name.data(), "rb");
	//��ȡͷ��
	fread(&nextLocation, sizeof(ll), 1, fp);
	ll location;
	while(num > 0){
		num--;
		if(nextLocation == -1){
			fseek(fp, 0, SEEK_END);
			location = ftell(fp);
			ll idx = (location - sizeof(ll)) / lineSeek+1;
			id.push_back(idx);
			break;
		}else{
			//������ͷ�ͱ�־λ
			fseek(fp, nextLocation+sizeof(ll)+sizeof(char), SEEK_SET);
			ll idx = (nextLocation)/lineSeek + 1;
			id.push_back(idx);
			fread(&nextLocation, sizeof(ll), 1, fp);
		}
	}
	ll n = id.size()-1;
	while(num > 0){
		num--;			
		id.push_back(id[n]+1);
		n++;
	}
	return  true;
}

int DataBase::update(string key, string value, string key2, string value2, string name){
	//�ж��Ƿ�ѡ��
	if (name == "" && (this->table_name == "δѡ��" || this->table_name == ""))
		return -2;
	vector<ll> id(0);
	vector< vector<string> > ans;
	this->query(key, value, id, ans);
	ll n = id.size();
	ll len = sizeof(char) + sizeof(ll) + this->table_col_pre_size[this->table_col_num]*sizeof(char);
	ll idx, selfLen, preSeek, lastSeek, totalSeek;
	if(!this->getKeyLocation(key2, idx, selfLen, preSeek, lastSeek, totalSeek)){
		//���޴��ֶ�
		return -1;
	}
	//�����ļ�����
	FILE * fp = fopen(this->table_name.data(), "rb+");
	for(int i = 0; i < n; i++){
		//������ͷ
		fseek(fp, sizeof(ll), SEEK_SET);
		//��ת������
		fseek(fp, (id[i]-1) * (totalSeek+sizeof(ll)+sizeof(char)), SEEK_CUR);
		//������Чλ������λ
		fseek(fp, sizeof(char) + sizeof(ll), SEEK_CUR);
		//������������λ��
		
		fseek(fp, preSeek, SEEK_CUR);
		//д��������
		
		fwrite(value2.data(), sizeof(char), selfLen, fp);
		//fflush(fp);
	}
	fclose(fp);
	return 0;
}

bool DataBase::getKeyLocation(string key, ll &idx, ll & selfLen, ll & preSeek, ll & lastSeek, ll & totalSeek){
	idx = -1;
	for(ll i = 0; i < this->table_col_num; i++){
		if(this->table_col_name[i] == key){
			idx = i;
		}
	} 
	if(idx == -1) return false; //�ֶβ�����
	//����ǰ�����ݳ��� 
	selfLen = this->table_col_size[idx];
	preSeek = (this->table_col_pre_size[idx] - this->table_col_pre_size[0])*sizeof(char);
	lastSeek = (this->table_col_pre_size[this->table_col_num] - this->table_col_pre_size[idx+1])*sizeof(char);
	totalSeek = this->table_col_pre_size[this->table_col_num] * sizeof(char);  //�����ܳ�
	return true;
}

void DataBase::emptyDataBase(){
	FILE * fp = fopen(settings::settings_name.data(), "wb");
	fclose(fp);
	fp = fopen(settings::table_settings_name.data(), "wb");
	fclose(fp);
}

void DataBase::emptyTable(){
	ll nextLocation = -1;
	FILE * fp = fopen(this->table_name.data(), "w");
	fwrite(&nextLocation, sizeof(ll), 1, fp);
	fclose(fp);
}



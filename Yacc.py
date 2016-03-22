# -*- coding: utf-8 -*-
import copy
#语法形式为pro->item 33 . 3 , 11
Gram = []
symbol = []
Var = []
tnt = 0
import sys   
sys.setrecursionlimit(1000000)
def closure(I):
	C = I;#I为语法的list
	#print C
	for c in C:
		strlist = c.split('->')[1].split(' ')
		ch = strlist[strlist.index('.')+1]
		#print 'ch: '+ch
		if ch != ',':
			if ch.isdigit():
				continue;
			first_arr = strlist[strlist.index('.')+2:len(strlist)]
			#print 'first1 : '+str(first_arr)
			first_arr.remove(',')
			first = First_str(first_arr)
			#print 'first: '+str(first)
			for item in Gram:
				#print 'pro: '+item.split('->')[0]
				if ch == item.split('->')[0]:
					#print 'aaa'
					for n in first:
						newi = ch + '->. '+item.split('->')[1]+' , '+n
						if newi not in C:
							C.append(newi);
	return C;

def Go(I,X):
	J = [];
	for it in I:
		strlist = it.split('->')[1].split(' ')
		ch = strlist[strlist.index('.')+1]
		if ch==X:
			J.append(changeIndex(it))
	return closure(J)

def changeIndex(Item):
	strlist = Item.split('->')[1].split(' ')
	i = strlist.index('.')
	strlist[i] = strlist[i+1]
	strlist[i+1] = '.'
	newi = Item.split('->')[0] + '->' + ' '.join(strlist)
	return newi

stateI = []
def CreatLR():
	it = Gram[0]
	one = it.split('->')[0] + '->. '+it.split('->')[1]+' , '+'45'
	t = []
	t.append(one)
	stateI.append(closure(t))
	for si in stateI:
		for x in symbol:
			I = Go(si,x)
			if len(I) > 0 and I not in stateI:
				stateI.append(I)
				global tnt
				#print I
				tnt+=1

def First_sig(x):
	first = []
	if x.isdigit():
		first.append(x)
	elif x == '45':
		first.append(x)
	else:
		for it in Gram:
			#print it
			if x == it.split('->')[0]:
				#print '------------------------------'
				#print 'x: '+x
				y = it.split('->')[1].split(' ')[0]				#print 'y: '+y
				tmp = First_sig(y)
				#print tmp
				if len(tmp) > 0:
					for t in tmp:
						if t not in first:
							first.append(t)
	return first

def First_str(x):
	return First_sig(x[0])
	
Action = {}
Goto = {}
def CreatTable():
	for item in stateI:
		if '$-><S> . , 45' in item:
			Action[(stateI.index(item),'45')] = ('E',0)
			continue;
		for it in item:
			strlist = it.split('->')[1].split(' ')
			ch = strlist[strlist.index('.')+1]
			if ch.isdigit():
				s = Go(item,ch)
				pos = stateI.index(s)
				Action[(stateI.index(item),ch)] = ('S',pos)
			if ch == ',':
				newstr = copy.deepcopy(strlist)
				newstr.pop(len(newstr)-1)
				newstr.remove('.')
				newstr.remove(',')
				pro = it.split('->')[0]+'->'+' '.join(newstr)
				pos = Gram.index(pro)
				Action[(stateI.index(item),strlist[len(strlist)-1])] = ('R',pos)
		for k in Var:
			s = Go(item,k)
			if len(s) > 0:
				pos = stateI.index(s)
				Goto[(stateI.index(item),k)] = pos
def loadGram():
	fp = open('gram.txt','r+')
	for line in fp:
		line = line.rstrip('\n \t');
		#print line
		Gram.append(line)
		pro = line.split('->')[0]
		if pro not in symbol:
			symbol.append(pro)
			Var.append(pro)
			#print 'Var'
		body = line.split('->')[1]
		body = body.split(' ')
		for it in body:
			if it not in symbol and it.isdigit():
				symbol.append(it)
				
	fp.close();

prolist = []
bodylist = []
def loadGram1():
	fp = open('in.txt','r+')
	for line in fp:
		line = line.rstrip('\n \t');
		if len(line)==0:
			continue
		#Gram.append(line)
		pro = line.split(':')[0]
		body = line.split(':')[1]
		body = body.split(' ')
		prolist.append(pro)
		bodylist.append(body)
		if pro not in symbol:
			symbol.append(pro)
			Var.append(pro)
	fp.close()

Ter = []
def dealT():
	for it in bodylist:
		for i in it:
			if i not in Var and i not in Ter:
				Ter.append(i)
				symbol.append(str(Ter.index(i)))
	for it in range(len(bodylist)):
		for i in range(len(bodylist[it])):
			if bodylist[it][i] in Ter:
				#print Ter.index(bodylist[it][i])
				bodylist[it][i] = str(Ter.index(bodylist[it][i]))
	for it in range(len(bodylist)):
		gm = prolist[it] + '->' + ' '.join(bodylist[it])
		Gram.append(gm)
		
loadGram()

"""
it = Gram[9]
#print Gram[9]
one = it.split('->')[0] + '->. '+it.split('->')[1]+' , '+'-1'
t = []
t.append(one)
#print t
print closure(t)

print len(Gram)
for it in Gram:
	print it
#print symbol
#print Var

CreatLR()
CreatTable()
print len(Action)
print 'GOTO: '
print len(Goto)


for it in Gram:
	one = it.split('->')[0] + '->. '+it.split('->')[1]+' , '+'#'
	t = []
	t.append(one)
	print closure(t)
"""


CreatLR()
CreatTable()
afp = open('Action.txt','w')
gfp = open('Goto.txt','w')
for ac in Action.keys():
	print str(ac)+' : '+str(Action[ac])
	afp.write(' '.join([str(ac[0]),ac[1]])+' '+' '.join([Action[ac][0],str(Action[ac][1])])+'\n')
print 'GOTO:'
for gt in Goto.keys():
	print str(gt)+' : '+str(Goto[gt])
	gfp.write(' '.join([str(gt[0]),str(Var.index(gt[1]))])+' '+str(Goto[gt])+'\n')
afp.close();
gfp.close()
fp = open('gram.txt','r+')
nfp = open('newg.txt','w')
for line in fp:
	line = line.rstrip('\n \t');
	pro = line.split('->')[0]
	body = line.split('->')[1]
	body = body.split(' ')
	nfp.write(str(Var.index(pro))+' '+str(len(body))+'\n')
nfp.close()
print len(stateI)
#print First_sig('<B>')